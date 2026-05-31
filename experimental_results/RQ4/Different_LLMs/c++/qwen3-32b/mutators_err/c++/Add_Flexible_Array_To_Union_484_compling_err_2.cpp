//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Flexible_Array_To_Union_484
 */ 
class MutatorFrontendAction_484 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(484)

private:
    class MutatorASTConsumer_484 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_484(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Flexible_Array_To_Union_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        if (!structDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       structDecl->getLocation()))
            return;
        if (!structDecl->isStruct() || structDecl->isLambda())
            return;
        if (!structDecl->isCompleteDefinition())
            return;
        auto field = Result.Nodes.getNodeAs<clang::FieldDecl>("field");
        if (!field || !field->getType()->isScalarType())
            return;
        auto structCode = stringutils::rangetoStr(*Result.SourceManager, structDecl->getSourceRange());
        if (structCode.empty())
            return;
        size_t closeBracePos = structCode.rfind('}');
        if (closeBracePos == string::npos)
            return;
        structCode.insert(closeBracePos, " char b[]");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(structDecl->getSourceRange()), structCode);
    } else if (auto *unionDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
        if (!unionDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       unionDecl->getLocation()))
            return;
        if (!unionDecl->isUnion())
            return;
        if (!unionDecl->isCompleteDefinition())
            return;
        auto unionCode = stringutils::rangetoStr(*Result.SourceManager, unionDecl->getSourceRange());
        if (unionCode.empty())
            return;
        size_t closeBracePos = unionCode.rfind('}');
        if (closeBracePos == string::npos)
            return;
        unionCode.insert(closeBracePos + 1, " u = {}");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(unionDecl->getSourceRange()), unionCode);
    }
}

void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = clang::ast_matchers::cxxRecordDecl(
        clang::ast_matchers::hasField(clang::ast_matchers::fieldDecl().bind("field")),
        clang::ast_matchers::unless(clang::ast_matchers::isUnion())
    ).bind("structDecl");
    DeclarationMatcher unionMatcher = clang::ast_matchers::cxxRecordDecl(
        clang::ast_matchers::isUnion()
    ).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.matchAST(Context);
}