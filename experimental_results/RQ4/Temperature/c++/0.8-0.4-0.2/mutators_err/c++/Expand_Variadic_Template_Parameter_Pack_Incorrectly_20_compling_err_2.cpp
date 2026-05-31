//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <memory>

/**
 * Expand_Variadic_Template_Parameter_Pack_Incorrectly_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_20>(TheRewriter);
    }

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
    };

    clang::Rewriter TheRewriter;
};

//source file
#include "../include/expand_variadic_template_parameter_pack_incorrectly_20.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<FunctionTemplateDecl>("FuncTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplatedDecl()->hasBody()) {
        auto body = FTD->getTemplatedDecl()->getBody();
        auto bodyText = TheRewriter.getRewrittenText(body->getSourceRange());
        
        // Find the position to insert the incorrect expansion
        size_t insertPos = bodyText.find("{") + 1;
        
        std::string incorrectExpansion = R"(
          /*mut20*/ auto incorrectLambda = [args...]<typename T>(T t) {
            auto nestedLambda = []<typename...>(Args... expandedArgs) {};
          };
        )";
        
        bodyText.insert(insertPos, incorrectExpansion);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateParameterCountIs(1))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}