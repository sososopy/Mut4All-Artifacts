//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Template_Parameters_With_Constraints_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Template_Parameters_With_Constraints_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (LE->getTemplateParameterList() == nullptr)
        return;
      auto template_parameters = LE->getTemplateParameterList();
      auto template_parameters_str = stringutils::rangetoStr(
          *(Result.SourceManager), template_parameters->getSourceRange());
      auto template_parameters_size = template_parameters->size();
      llvm::outs() << template_parameters_size << '\n';
      if (template_parameters_size == 0)
        return;
      int index = getrandom::getRandomIndex(template_parameters_size - 1);
      llvm::outs() << index << '\n';
      auto target_parameter = template_parameters->getParam(index);
      auto target_parameter_str = stringutils::rangetoStr(
          *(Result.SourceManager), target_parameter->getSourceRange());
      llvm::outs() << target_parameter_str << '\n';
      string constraint = "std::is_integral_v<" + target_parameter_str + ">";
      string requires_clause = " requires " + constraint;
      llvm::outs() << requires_clause << '\n';
      auto lambda_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                LE->getSourceRange());
      lambda_str.insert(lambda_str.find('{'), requires_clause);
      lambda_str = "/*mut48*/" + lambda_str;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambda_str);
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}