//header file
#pragma once
#include "Mutator_base.h"

/**
 * Omit_TemplateFunctionCallOperator_388
 */ 
class MutatorFrontendAction_388 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(388)

private:
    class MutatorASTConsumer_388 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_388(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Omit_TemplateFunctionCallOperator_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>(("CallExpr"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getDirectCallee()) {
        auto callee = MT->getDirectCallee();
        if (callee->getKind() == Decl::Kind::TemplateSpecialization) {
          auto templateSpec = cast<TemplateSpecialization>(callee);
          if (templateSpec->getTemplate()->getKind() == Decl::Kind::FunctionTemplate) {
            auto funcTemplate = cast<FunctionTemplate>(templateSpec->getTemplate());
            if (funcTemplate->getTemplatedDecl()->getKind() == Decl::Kind::CXXMethod) {
              auto method = cast<CXXMethod>(funcTemplate->getTemplatedDecl());
              if (method->isInstance() && !method->isStatic()) {
                // Omit the function call operator
                auto mutatedDeclaration = declaration.substr(0, declaration.find("("));
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}