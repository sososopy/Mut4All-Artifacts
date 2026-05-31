//header file
#pragma once
#include "Mutator_base.h"
#include <map>
#include <string>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>

/**
 * Replace_Operator_With_Overloaded_380
 */ 
class MutatorFrontendAction_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(380)

private:
    class MutatorASTConsumer_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_380(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::map<const clang::CXXRecordDecl*, std::string> overloadedOperators;
    };
};

//source file
#include "../include/replace_operator_with_overloaded_380.h"
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/ExprCXX.h>
#include <clang/Rewrite/Core/Rewriter.h>

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *OD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorDecl")) {
        if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       OD->getLocation()))
            return;

        auto recordDecl = OD->getParent();
        if (!recordDecl || !recordDecl->isCompleteDefinition())
            return;

        std::string operatorName = OD->getNameAsString();
        if (operatorName.find("operator") == std::string::npos)
            return;

        overloadedOperators[recordDecl] = operatorName;
    } else if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getBeginLoc()))
            return;

        auto lhsType = BO->getLHS()->getType();
        if (auto *recordType = lhsType->getAsCXXRecordDecl()) {
            auto it = overloadedOperators.find(recordType);
            if (it != overloadedOperators.end()) {
                std::string replacement = it->second + "(" +
                                          Rewrite.getRewrittenText(BO->getLHS()->getSourceRange()) + ", " +
                                          Rewrite.getRewrittenText(BO->getRHS()->getSourceRange()) + ")";
                Rewrite.ReplaceText(BO->getSourceRange(), replacement);
            }
        }
    }
}

void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher operatorMatcher = cxxMethodDecl(isOverloadedOperator()).bind("OperatorDecl");
    StatementMatcher binaryOpMatcher = binaryOperator().bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(operatorMatcher, &callback);
    matchFinder.addMatcher(binaryOpMatcher, &callback);
    matchFinder.matchAST(Context);
}