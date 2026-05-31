//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/LangOptions.h"

/**
 * Template_Reference_Mismatch_Function_Call_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Reference_Mismatch_Function_Call_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getBeginLoc()))
            return;

        FunctionDecl *FD = Call->getDirectCallee();
        if (!FD || !FD->isVariadic() || FD->getNumParams() != 1)
            return;

        const ParmVarDecl *Param = FD->getParamDecl(0);
        if (!Param->isParameterPack())
            return;

        QualType paramType = Param->getType();
        if (!paramType->isRValueReferenceType())
            return;

        ASTContext *Context = Result.Context;
        SourceManager &SM = Context->getSourceManager();
        LangOptions &LangOpts = Context->getLangOpts();

        std::string templateArgs;
        for (unsigned i = 0; i < Call->getNumArgs(); ++i) {
            QualType argType = Call->getArg(i)->getType();
            std::string typeName = Context->getCanonicalType(argType).getAsString();
            if (!templateArgs.empty())
                templateArgs += ", ";
            templateArgs += typeName;
        }

        std::string callCode = Lexer::getSourceText(CharSourceRange::getTokenRange(Call->getSourceRange()), SM, LangOpts).str();
        size_t openParenPos = callCode.find('(');
        if (openParenPos == std::string::npos)
            return;

        std::string funcNamePart = callCode.substr(0, openParenPos);
        std::string argsPart = callCode.substr(openParenPos);

        std::string newCode = funcNamePart + "<" + templateArgs + ">" + argsPart;

        SourceLocation startLoc = Call->getBeginLoc();
        SourceLocation endLoc = Call->getEndLoc();
        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newCode);
    }
}

void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr().bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}