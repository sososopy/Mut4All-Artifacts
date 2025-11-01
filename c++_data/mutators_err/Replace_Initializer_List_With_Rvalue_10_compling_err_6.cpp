//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Replace_Initializer_List_With_Rvalue_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_initializer_list_with_rvalue_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("initializerListVar")) {
        if (!Result.SourceManager->isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->getType()->isSpecificBuiltinType(BuiltinType::Int)) {
            if (const auto *initExpr = VD->getInit()) {
                if (const auto *initListExpr = dyn_cast<InitListExpr>(initExpr)) {
                    std::string newInit = "std::initializer_list<int>({";
                    for (unsigned i = 0; i < initListExpr->getNumInits(); ++i) {
                        if (i != 0) newInit += ", ";
                        newInit += Lexer::getSourceText(CharSourceRange::getTokenRange(initListExpr->getInit(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
                    }
                    newInit += "}).begin()";
                    Rewrite.ReplaceText(initExpr->getSourceRange(), "auto it = " + newInit + ";");
                }
            }
        }
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr()), hasType(isSpecificBuiltinType(BuiltinType::Int))).bind("initializerListVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}