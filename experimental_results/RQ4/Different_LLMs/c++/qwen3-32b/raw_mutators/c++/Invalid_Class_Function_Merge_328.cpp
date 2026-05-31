//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Class_Function_Merge_328
 */ 
class MutatorFrontendAction_328 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(328)
private:
    class MutatorASTConsumer_328 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_328(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *currentClass = nullptr;
    };
};

//source file
#include "../include/invalid_class_function_merge_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *classDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classDecl")) {
        if (!classDecl || !Result.Context->getSourceManager().isWrittenInMainFile(classDecl->getLocation()))
            return;
        if (classDecl->isLambda() || classDecl->isTemplated() || !classDecl->isCompleteDefinition())
            return;
        currentClass = classDecl;
    } else if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getLocation()))
            return;
        if (funcDecl->isCXXClassMember() || !funcDecl->hasBody())
            return;
        if (!currentClass)
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceLocation classEnd = currentClass->getEndLoc();
        SourceLocation funcStart = funcDecl->getBeginLoc();

        if (SM.isBeforeInTranslationUnit(funcStart, classEnd)) {
            return; // function is before the class
        }

        std::string funcText = stringutils::rangetoStr(SM, funcDecl->getSourceRange());
        Rewrite.ReplaceText(classEnd, 1, funcText);
        Rewrite.RemoveText(funcDecl->getSourceRange());
        currentClass = nullptr;
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("classDecl");
    DeclarationMatcher functionMatcher = functionDecl(unless(isCXXClassMember()), hasBody()).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}