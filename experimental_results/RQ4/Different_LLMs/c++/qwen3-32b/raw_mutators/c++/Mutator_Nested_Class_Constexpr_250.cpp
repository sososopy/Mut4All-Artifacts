//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Nested_Class_Constexpr_250
 */ 
class MutatorFrontendAction_250 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(250)

private:
    class MutatorASTConsumer_250 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_250(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Nested_Class_Constexpr_250.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
            std::string varName = VD->getNameAsString();
            QualType varType = VD->getType();
            QualType returnType = varType->isReferenceType() ? varType.getPointeeType() : varType;
            std::string returnTypeName = returnType.getUnqualifiedType().getAsString();
            SourceLocation insertLoc = VD->getEndLoc();
            std::string structCode = "    struct Temp {\n        constexpr " + returnTypeName + " get() { return " + varName + "; }\n    };\n";
            Rewrite.InsertTextAfterToken(insertLoc, structCode);
        }
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        hasDescendant(
            varDecl(
                hasType(referenceType(isRValueReference())).bind("var")
            )
        )
    ).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}