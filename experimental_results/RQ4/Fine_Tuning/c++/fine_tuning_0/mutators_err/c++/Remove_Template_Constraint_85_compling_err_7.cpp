//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Constraint_85
 */ 
class MutatorFrontendAction_85 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(85)

private:
    class MutatorASTConsumer_85 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_85(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Template_Constraint_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("MutTemp")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto tempdecl = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      int pos = tempdecl.find("template <");
      int pos2 = tempdecl.find("typename", pos);
      tempdecl.replace(pos + 10, pos2 - pos - 10, "typename ");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), tempdecl);
    }
}
  
void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasAnyTemplateArgument(has(hasType(qualType(hasDeclaration(decl().bind("concept"))))))).bind("MutTemp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}