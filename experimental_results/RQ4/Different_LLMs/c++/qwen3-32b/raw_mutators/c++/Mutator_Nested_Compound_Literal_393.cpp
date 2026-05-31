//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Nested_Compound_Literal_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Nested_Compound_Literal_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>("stringLiteral")) {
        if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(SL->getLocation()))
            return;
        auto *Parent = Result.Context->getParents(*SL).front().get<clang::InitListExpr>();
        if (!Parent)
            return;
        clang::ArrayType *AT = Parent->getType()->getAsArrayType();
        if (!AT)
            return;
        clang::QualType ElementType = AT->getElementType();
        std::string Str = SL->getString().str();
        std::string chars;
        for (size_t i = 0; i < Str.size(); ++i) {
            chars += "'";
            chars += Str[i];
            chars += "', ";
        }
        chars += "'\\0'";
        std::string replacement = "(" + ElementType.getAsString() + "[]){ " + chars + " }";
        Rewrite.ReplaceText(SL->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = stringLiteral().bind("stringLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}