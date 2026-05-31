//header file
#pragma once
#include "Mutator_base.h"
#include <vector>
#include <string>

/**
 * Replace_This_Parameter_Type_In_Defaulted_OperatorEq_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> user_defined_types;
    };
};

//source file
#include "../include/Replace_This_Parameter_Type_In_Defaulted_OperatorEq_197.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <vector>
#include <string>
#include <random>

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        if (method->getNumParams() != 2)
            return;

        clang::ParmVarDecl* firstParam = method->getParamDecl(0);
        if (!firstParam)
            return;

        clang::SourceRange typeRange = firstParam->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;

        std::vector<std::string> possible_types = {"int", "bool", "char", "float", "double", "short", "long", "unsigned int", "unsigned char", "unsigned short", "unsigned long"};
        possible_types.insert(possible_types.end(), user_defined_types.begin(), user_defined_types.end());

        int index = getrandom::getRandomIndex(possible_types.size());
        std::string new_type = possible_types[index];

        Rewrite.ReplaceText(typeRange, new_type);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        user_defined_types.push_back(RD->getNameAsString());
    }
}
  
void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher methodMatcher = 
        clang::ast_matchers::cxxMethodDecl(
            clang::ast_matchers::hasName("operator=="),
            clang::ast_matchers::isDefaulted(),
            clang::ast_matchers::hasNumParameters(2)
        ).bind("method");

    clang::ast_matchers::DeclarationMatcher recordMatcher = 
        clang::ast_matchers::cxxRecordDecl().bind("recordDecl");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}