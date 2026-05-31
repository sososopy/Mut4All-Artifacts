//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * reorder_class_declaration_657
 */ 
class MutatorFrontendAction_657 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(657)

private:
    class MutatorASTConsumer_657 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_657(Rewriter &R) : TheRewriter(R) {}
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
#include "MutatorFrontendAction_657.h"

void MutatorFrontendAction_657::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<CXXRecordDecl>("ClassDecl")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
            return;
        
        if (!CL->isCompleteDefinition())
            return;

        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation classEndLoc = CL->getEndLoc();
        SourceLocation classStartLoc = CL->getBeginLoc();

        // Move class declaration up one line
        SourceLocation prevLineLoc = SM.translateLineCol(SM.getFileID(classStartLoc), SM.getSpellingLineNumber(classStartLoc) - 1, 1);
        std::string classText = Lexer::getSourceText(CharSourceRange::getTokenRange(CL->getSourceRange()), SM, LangOptions()).str();
        Rewrite.InsertText(prevLineLoc, classText + "\n", true, true);
        Rewrite.RemoveText(CL->getSourceRange());

        // Remove semicolon after class definition
        SourceLocation semiLoc = Lexer::findLocationAfterToken(classEndLoc, tok::semi, SM, LangOptions(), false);
        if (semiLoc.isValid()) {
            Rewrite.RemoveText(semiLoc, 1);
        }
    }
}

void MutatorFrontendAction_657::MutatorASTConsumer_657::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}