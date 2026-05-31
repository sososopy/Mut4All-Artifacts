//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Const_Variable_In_Openmp_Directive_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Const_Variable_In_Openmp_Directive_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VariableDeclarations")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if the variable is const and used within an OpenMP directive
      if (VD->getType().isConstQualified() && VD->hasAttr<OMPParallelAttr>() || VD->hasAttr<OMPTaskAttr>()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        // Remove the const keyword
        declaration = std::regex_replace(declaration, std::regex("const\\s+"), "");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("VariableDeclarations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}