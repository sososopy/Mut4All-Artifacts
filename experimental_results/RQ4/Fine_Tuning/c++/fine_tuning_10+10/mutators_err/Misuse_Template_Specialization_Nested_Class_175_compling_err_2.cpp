//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Template_Specialization_Nested_Class_175
 */ 
class MutatorFrontendAction_175 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(175)

private:
    class MutatorASTConsumer_175 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_175(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Misuse_Template_Specialization_Nested_Class_175.h"

// ========================================================================================================
#define MUT175_OUTPUT 1

void MutatorFrontendAction_175::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("NestedTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getBeginLoc()))
        return;

      if (MT->getNumTemplateArgs() > 0) {
        const clang::TemplateArgument &templateArg = MT->getTemplateArgs()[0];
        if (templateArg.getKind() == clang::TemplateArgument::Integral) {
          auto typeName = MT->getQualifier()->getAsType()->getAs<clang::TemplateSpecializationType>()->getArg(0).getAsType();
          std::string invalidParam = typeName.getAsString();
          auto sourceRange = MT->getSourceRange();
          std::string replacement = "Outer<" + invalidParam + ">::template Inner<" + invalidParam + ">";
          Rewrite.ReplaceText(sourceRange, replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_175::MutatorASTConsumer_175::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxDependentScopeMemberExpr(hasMemberName("Inner")).bind("NestedTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}