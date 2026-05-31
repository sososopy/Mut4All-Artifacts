#pragma once
#include "Mutator_base.h"

/**
 * Replace_Cpp_Only_Constructs_In_Non_Cpp_Contexts_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(238)
private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const StaticAssertDecl *> targetStaticAsserts;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      targetStaticAsserts.push_back(SA);
    }
    else if (auto *TA = Result.Nodes.getNodeAs<clang::TemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto declText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TA->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template declaration with a non-template equivalent using int
      std::string replacement = "int " + TA->getNameAsString() + " = 0;";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), replacement);
    }
}

void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher staticAssertMatcher = staticAssertDecl().bind("StaticAssert");
    DeclarationMatcher templateMatcher = templateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(staticAssertMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
    
    // Process recorded static asserts after matching
    for (const StaticAssertDecl *SA : callback.targetStaticAsserts) {
        SourceManager &SM = TheRewriter.getSourceMgr();
        if (!SM.isWrittenInMainFile(SA->getLocation()))
            continue;
        
        // Check if the static assert uses C++-only features
        const Expr *AssertExpr = SA->getAssertExpr();
        if (!AssertExpr)
            continue;
        
        // Replace with a trivial static assertion that is always true
        std::string replacement = "_Static_assert(1)";
        TheRewriter.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), replacement);
    }
}