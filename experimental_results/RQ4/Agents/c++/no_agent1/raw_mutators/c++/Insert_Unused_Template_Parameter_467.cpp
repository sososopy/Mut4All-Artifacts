//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)

private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/insert_unused_template_parameter_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Ensure the function is not a template specialization
      if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
        return;

      //Get the source code text of target node
      auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string templateParam = "template<typename UnusedParam>\n";
      functionText.insert(0, templateParam);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}