//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_initialization_with_multiple_initializers_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::RecordDecl*, const clang::FieldDecl*> unionFields;
    };
};

//source file
#include "../include/modify_union_initialization_with_multiple_initializers_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
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
        std::string originalInit = stringutils::rangetoStr(*(Result.SourceManager), InitExpr->getSourceRange());
        std::string additionalInit = ", ." + AdditionalField->getNameAsString() + " = " + getDefaultValueForType(AdditionalField->getType());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedInit = originalInit.substr(0, originalInit.size() - 1) + additionalInit + originalInit.back();
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitExpr->getSourceRange()), mutatedInit);
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher unionMatcher = recordDecl(isUnion()).bind("UnionDecl");
    StatementMatcher initMatcher = initListExpr(hasType(recordDecl(isUnion()).bind("UnionType"))).bind("UnionInit");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(initMatcher, &callback);
    matchFinder.matchAST(Context);
}

std::string getDefaultValueForType(const clang::QualType &type) {
    if (type->isIntegerType())
        return "0";
    else if (type->isFloatingType())
        return "0.0";
    else if (type->isPointerType())
        return "nullptr";
    else
        return "{}"; // Default for complex types or structs
}