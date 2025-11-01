//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_Return_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_template_specialization_return_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("specializedFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() && FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        if (body) {
          //Perform mutation on the source code text by applying string replacement
          auto returnStmt = dyn_cast<clang::ReturnStmt>(body->child_begin().getNode());
          if (returnStmt) {
            auto returnRange = returnStmt->getSourceRange();
            std::string mutatedReturn = "return some_undeclared_variable;";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(returnRange), mutatedReturn);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("specializedFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}