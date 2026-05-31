//source file
#include "../include/modify_union_initialization_with_multiple_initializers_26.h"
#include <clang/Lex/Lexer.h>

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
        std::string originalInit = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(InitExpr->getSourceRange()),
            *(Result.SourceManager), Result.Context->getLangOpts()).str();
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