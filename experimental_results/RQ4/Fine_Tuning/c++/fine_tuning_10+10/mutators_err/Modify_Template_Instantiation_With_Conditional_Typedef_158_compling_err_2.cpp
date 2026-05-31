//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_With_Conditional_Typedef_158
 */ 
class MutatorFrontendAction_158 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(158)

private:
    class MutatorASTConsumer_158 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_158(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_instantiation_with_conditional_typedef_158.h"

// ========================================================================================================
#define MUT158_OUTPUT 1

void MutatorFrontendAction_158::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::Type>("templateTypedef")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getAsCXXRecordDecl()->getBeginLoc()))
        return;

      if (auto *TST = llvm::dyn_cast<TemplateSpecializationType>(MT)) {
        auto templateName = TST->getTemplateName().getAsTemplateDecl()->getNameAsString();
        if (templateName == "enable_if") {
          auto args = TST->template_arguments();
          if (args.size() > 0 && args[0].getKind() == TemplateArgument::Type) {
            auto conditionType = args[0].getAsType().getAsString();
            std::string newCondition = "sizeof(" + conditionType + ") > -1";
            std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), TST->getSourceRange());
            std::string mutatedText = originalText;
            size_t pos = mutatedText.find(conditionType);
            if (pos != std::string::npos) {
              mutatedText.replace(pos, conditionType.length(), newCondition);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(TST->getSourceRange()), mutatedText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_158::MutatorASTConsumer_158::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    TypeMatcher matcher = type().bind("templateTypedef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}