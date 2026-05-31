//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Enum_Param_213
 */ 
class MutatorFrontendAction_213 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(213)

private:
    class MutatorASTConsumer_213 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_213(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Invalid_Enum_Param_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto *ClassDecl = MT->getParent();
        if (!ClassDecl || !ClassDecl->isClass())
            return;

        std::string EnumName;
        for (auto *D : ClassDecl->decls()) {
            if (auto *ED = dyn_cast<clang::EnumDecl>(D)) {
                EnumName = ED->getNameAsString();
                break;
            }
        }
        if (EnumName.empty())
            return;

        const clang::FunctionType *FT = MT->getType()->getAs<clang::FunctionType>();
        if (!FT)
            return;

        clang::FunctionProtoTypeLoc *FTLoc = MT->getTypeSourceInfo()->getTypeLoc().getAs<clang::FunctionProtoTypeLoc>();
        if (!FTLoc)
            return;
        clang::SourceLocation CloseLoc = FTLoc->getRParenLoc();
        if (CloseLoc.isInvalid())
            return;

        std::string InsertText = ", enum " + EnumName + ";";
        Rewrite.InsertTextBefore(CloseLoc, InsertText);
    }
}
  
void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::cxxMethodDecl(clang::ast_matchers::unless(clang::ast_matchers::cxxConstructorDecl())).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}