//header file
#pragma once
#include "Mutator_base.h"
#include <map>
#include <string>
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Tooling.h>

/**
 * modify_union_initialization_with_multiple_initializers_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
        std::map<const clang::RecordDecl*, const clang::FieldDecl*> unionFields;
        std::string getDefaultValueForType(const clang::QualType &type);
    };
};

//source file
#include "../include/modify_union_initialization_with_multiple_initializers_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitExpr = Result.Nodes.getNodeAs<clang::InitListExpr>("UnionInit")) {
        //Filter nodes in header files
        if (!InitExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                InitExpr->getBeginLoc()))
            return;

        const clang::Type *UnionType = InitExpr->getType().getTypePtr();
        if (!UnionType || !UnionType->isUnionType()) 
            return;
        
        const clang::RecordDecl *UnionDecl = UnionType->getAsUnionType()->getDecl();
        if (!UnionDecl || unionFields.find(UnionDecl) == unionFields.end())
            return;

        const clang::FieldDecl *AdditionalField = unionFields[UnionDecl];
        if (!AdditionalField)
            return;

        //Get the source code text of target node
        std::string originalInit = clang::tooling::stringutils::rangeToStr(*(Result.SourceManager), InitExpr->getSourceRange());
        std::string additionalInit = ", ." + AdditionalField->getNameAsString() + " = " + getDefaultValueForType(AdditionalField->getType());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedInit = originalInit.substr(0, originalInit.size() - 1) + additionalInit + originalInit.back();
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(InitExpr->getSourceRange()), mutatedInit);
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher unionMatcher = clang::ast_matchers::recordDecl(clang::ast_matchers::isUnion()).bind("UnionDecl");
    clang::ast_matchers::StatementMatcher initMatcher = clang::ast_matchers::initListExpr(clang::ast_matchers::hasType(clang::ast_matchers::recordDecl(clang::ast_matchers::isUnion()).bind("UnionType"))).bind("UnionInit");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(initMatcher, &callback);
    matchFinder.matchAST(Context);
}

std::string MutatorFrontendAction_26::Callback::getDefaultValueForType(const clang::QualType &type) {
    if (type->isIntegerType())
        return "0";
    else if (type->isFloatingType())
        return "0.0";
    else if (type->isPointerType())
        return "nullptr";
    else
        return "{}"; // Default for complex types or structs
}