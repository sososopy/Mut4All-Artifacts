//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Ctor_Call_With_Template_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Ctor_Call_With_Template_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ctorCall = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ctor_call")) {
        if (!ctorCall || !Result.Context->getSourceManager().isWrittenInMainFile(ctorCall->getBeginLoc()))
            return;

        if (auto *argExpr = Result.Nodes.getNodeAs<clang::DeclRefExpr>("arg")) {
            QualType argType = argExpr->getType();
            std::string argTypeName = argType.getAsString(*Result.Context->getPrintingPolicy());

            // Generate unique function name based on type
            std::string funcName = "getCallableWrapper_" + argTypeName;
            bool funcExists = false;

            for (Decl *decl : Result.Context->getTranslationUnitDecl()->decls()) {
                if (FunctionDecl *FD = dyn_cast<FunctionDecl>(decl)) {
                    if (FD->getNameAsString() == funcName && 
                        FD->getResultType().getCanonicalType() == argType.getCanonicalType()) {
                        funcExists = true;
                        break;
                    }
                }
            }

            if (!funcExists) {
                std::string funcDecl = argTypeName + " " + funcName + "();";
                std::string funcDef = argTypeName + " " + funcName + "() { return {}; }";
                SourceLocation endOfFile = Result.SourceManager->getLocForEndOfFile(Result.SourceManager->getMainFileID());
                Rewrite.InsertText(endOfFile, "\n" + funcDecl + "\n" + funcDef + "\n", true, true);
            }

            SourceRange argRange = argExpr->getSourceRange();
            Rewrite.ReplaceText(argRange, funcName + "()");
        }
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructExpr(
        hasDescendant(declRefExpr().bind("arg"))
    ).bind("ctor_call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}