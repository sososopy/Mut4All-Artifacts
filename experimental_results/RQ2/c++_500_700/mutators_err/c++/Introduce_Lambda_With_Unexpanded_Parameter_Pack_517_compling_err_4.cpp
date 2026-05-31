//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * introduce_lambda_with_unexpanded_parameter_pack_517
 */ 
class MutatorFrontendAction_517 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(517)

private:
    class MutatorASTConsumer_517 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_517(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_lambda_with_unexpanded_parameter_pack_517.h"

// ========================================================================================================
#define MUT517_OUTPUT 1

void MutatorFrontendAction_517::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FN = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithPack")) {
      if (!FN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FN->getLocation()))
        return;

      if (FN->isTemplateInstantiation())
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FN->getSourceRange());

      size_t bodyPos = declaration.find("{");
      if (bodyPos != std::string::npos) {
        std::string lambdaCode = "\n/*mut517*/auto lambda = [] consteval -> void { (void)(sizeof...(Ts)); };\nlambda();\n";
        declaration.insert(bodyPos + 1, lambdaCode);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FN->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_517::MutatorASTConsumer_517::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAnyTemplateParameter(anything())).bind("FunctionWithPack");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}