//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * change_operator_return_type_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/change_operator_return_type_22.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isOverloadedOperator() && (MT->getNumParams() <= 2)) {
        std::string declaration = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(MT->getSourceRange()),
            *(Result.SourceManager), clang::LangOptions(), 0).str();

        std::string returnType = "Unrelated";
        std::string newBody = "return Unrelated();";
        
        size_t pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration.replace(0, pos, returnType + " " + MT->getNameAsString());
          declaration.insert(pos + 1, "\n" + newBody + "\n");
        }
        
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator(), hasAnyOperatorName("+", "-", "*", "/", "%", "==", "!=", "<", ">", "<=", ">=")).bind("OperatorMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}