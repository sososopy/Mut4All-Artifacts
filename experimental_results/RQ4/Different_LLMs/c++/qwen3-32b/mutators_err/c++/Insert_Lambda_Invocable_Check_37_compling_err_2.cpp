//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Invocable_Check_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
        std::string className;
        std::string methodName;
    };
};

//source file
#include "../include/Insert_Lambda_Invocable_Check_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;
        const CXXRecordDecl *classDecl = MD->getParent();
        if (!classDecl || classDecl->getNameAsString().empty())
            return;
        className = classDecl->getNameAsString();
        methodName = MD->getNameAsString();
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("mainFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        const Stmt *body = FD->getBody();
        if (!body)
            return;
        SourceLocation endLoc = body->getEndLoc();
        if (className.empty() || methodName.empty())
            return;
        std::string code = "    auto lambda = [](const auto& cls) -> decltype( cls." + methodName + "() ) {};\n    auto isInv = std::is_invocable_v<decltype(lambda), " + className + ">;\n    return 0;\n";
        Rewrite.InsertText(endLoc, code, true, true);
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher methodMatcher = cxxMethodDecl(
        unless(clang::ast_matchers::isStatic()),
        unless(cxxConstructorDecl()),
        unless(cxxDestructorDecl())
    ).bind("method");
    DeclarationMatcher mainMatcher = functionDecl(
        hasName("main"),
        hasParameter(0, hasType(isInteger())),
        hasParameter(1, hasType(pointerType(pointee(arrayType()))))
    ).bind("mainFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(mainMatcher, &callback);
    matchFinder.matchAST(Context);
}