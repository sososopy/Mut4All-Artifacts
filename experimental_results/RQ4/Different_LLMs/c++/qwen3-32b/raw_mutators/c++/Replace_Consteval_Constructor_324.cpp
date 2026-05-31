//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Consteval_Constructor_324
 */ 
class MutatorFrontendAction_Replace_Consteval_Constructor_324 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Consteval_Constructor_324)
private:
    class MutatorASTConsumer_Replace_Consteval_Constructor_324 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Consteval_Constructor_324(Rewriter &R) : TheRewriter(R) {}
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
        bool hasExternFunc = false;
    };
};

//source file
#include "../include/Replace_Consteval_Constructor_324.h"

// ========================================================================================================
#define MUT_Replace_Consteval_Constructor_324_OUTPUT 1

void MutatorFrontendAction_Replace_Consteval_Constructor_324::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        if (!CD->hasBody())
            return;

        ASTContext *Context = Result.Context;
        SourceManager &SM = *Context->getSourceManager();
        hasExternFunc = false;

        for (auto *D : Context->getTranslationUnitDecl()->decls()) {
            if (auto *FD = dyn_cast<FunctionDecl>(D)) {
                if (FD->hasAttr<ConstexprAttr>() && FD->getReturnType()->isVoidType() && FD->getNameAsString() == "extern_func") {
                    hasExternFunc = true;
                    break;
                }
            }
        }

        SourceLocation endLoc = SM.getEndOfFile(SM.getFileID(CD->getLocation()));
        auto className = CD->getParent()->getNameAsString();

        if (!hasExternFunc) {
            Rewrite.InsertText(endLoc, "\nextern constexpr void extern_func();\n", true, true);
        }

        auto original = stringutils::rangetoStr(*Result.SourceManager, CD->getSourceRange());
        std::string modified = "consteval " + original;
        size_t openBracePos = modified.find('{');
        if (openBracePos != std::string::npos) {
            modified.insert(openBracePos + 1, "extern_func();\n");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), modified);

        std::string constinitDecl = "\nconstinit " + className + " obj;\n";
        Rewrite.InsertText(endLoc, constinitDecl, true, true);
    }
}
  
void MutatorFrontendAction_Replace_Consteval_Constructor_324::MutatorASTConsumer_Replace_Consteval_Constructor_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}