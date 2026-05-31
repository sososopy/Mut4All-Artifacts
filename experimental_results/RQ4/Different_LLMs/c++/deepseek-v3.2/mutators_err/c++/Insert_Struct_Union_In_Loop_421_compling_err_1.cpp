//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Struct_Union_In_Loop_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)
private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<std::string> typedefNames;
    };
};

//source file
#include "../include/Mutator_Insert_Struct_Union_In_Loop_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        typedefNames.push_back(TD->getNameAsString());
    } else if (auto *FL = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
        if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(FL->getLocation()))
            return;
        if (typedefNames.empty()) return;
        std::string typedefName = typedefNames[getrandom::getRandomIndex(typedefNames.size() - 1)];
        clang::Expr *Cond = FL->getCond();
        std::string insertion;
        if (!Cond) {
            insertion = "struct " + typedefName + " {int x;} dummy";
        } else {
            std::string condText = stringutils::rangetoStr(*(Result.SourceManager), Cond->getSourceRange());
            insertion = "struct " + typedefName + " {int x;} dummy, " + condText;
        }
        SourceLocation insertLoc = FL->getCond() ? FL->getCond()->getBeginLoc() : FL->getRParenLoc();
        Rewrite.InsertTextBefore(insertLoc, insertion);
    } else if (auto *WL = Result.Nodes.getNodeAs<clang::WhileStmt>("WhileLoop")) {
        if (!WL || !Result.Context->getSourceManager().isWrittenInMainFile(WL->getLocation()))
            return;
        if (typedefNames.empty()) return;
        std::string typedefName = typedefNames[getrandom::getRandomIndex(typedefNames.size() - 1)];
        clang::Expr *Cond = WL->getCond();
        std::string insertion;
        if (!Cond) {
            insertion = "struct " + typedefName + " {int x;} dummy";
        } else {
            std::string condText = stringutils::rangetoStr(*(Result.SourceManager), Cond->getSourceRange());
            insertion = "struct " + typedefName + " {int x;} dummy, " + condText;
        }
        SourceLocation insertLoc = WL->getCond()->getBeginLoc();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Cond->getSourceRange()), insertion);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DoStmt>("DoLoop")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (typedefNames.empty()) return;
        std::string typedefName = typedefNames[getrandom::getRandomIndex(typedefNames.size() - 1)];
        clang::Expr *Cond = DL->getCond();
        std::string condText = stringutils::rangetoStr(*(Result.SourceManager), Cond->getSourceRange());
        std::string insertion = "struct " + typedefName + " {int x;} dummy, " + condText;
        SourceLocation insertLoc = DL->getCond()->getBeginLoc();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Cond->getSourceRange()), insertion);
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher typedefMatcher = typedefDecl().bind("TypedefDecl");
    StatementMatcher forMatcher = forStmt().bind("ForLoop");
    StatementMatcher whileMatcher = whileStmt().bind("WhileLoop");
    StatementMatcher doMatcher = doStmt().bind("DoLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.addMatcher(forMatcher, &callback);
    matchFinder.addMatcher(whileMatcher, &callback);
    matchFinder.addMatcher(doMatcher, &callback);
    matchFinder.matchAST(Context);
}