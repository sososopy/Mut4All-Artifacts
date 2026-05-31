//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_Forward_Declared_Enum_In_Static_Assert_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> forward_enums;
    };
};

//source file
#include "../include/use_forward_declared_enum_in_static_assert_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAsserts")) {
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      auto cond = SA->getAssertCondition();
      if (cond->isValueDependent() || cond->isInstantiationDependent())
        return;
      if (forward_enums.empty())
        return;
      size_t index = getrandom::getRandomIndex(forward_enums.size() - 1);
      auto target = forward_enums[index];
      auto ins = "/*mut31*/!std::is_function<" + target->getNameAsString() +
                 ">::value";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(cond->getSourceRange()),
                          ins);
    } else if (auto *ED =
                   Result.Nodes.getNodeAs<clang::EnumDecl>("ForwardEnums")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (!ED->isScoped() && !ED->isFixed())
        return;
      if (ED->isCompleteDefinition())
        return;
      forward_enums.push_back(ED);
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = staticAssertDecl().bind("StaticAsserts");
    auto forward_enum_matcher = enumDecl().bind("ForwardEnums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(forward_enum_matcher, &callback);
    matchFinder.matchAST(Context);
}