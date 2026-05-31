//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variable_With_Global_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
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
        std::string globalVarName;
    };
};

//source file
#include "../include/replace_variable_with_global_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("LocalVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (VD->isLocalVarDecl() && VD->hasInit()) {
            globalVarName = "global_" + VD->getNameAsString();
            std::string globalDecl = "int " + globalVarName + " = " +
                                     stringutils::rangetoStr(*(Result.SourceManager), VD->getInit()->getSourceRange()) + ";\n";
            Rewrite.InsertTextBefore(VD->getBeginLoc(), globalDecl);
            Rewrite.ReplaceText(VD->getSourceRange(), globalVarName);
        }
    } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RS->getBeginLoc()))
            return;

        if (!globalVarName.empty()) {
            std::string returnReplacement = "return " + globalVarName;
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), returnReplacement);
        }
    }
}

void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher localVarMatcher = varDecl(hasInitializer(anything()), isLocalVarDecl()).bind("LocalVar");
    StatementMatcher returnStmtMatcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(localVarMatcher, &callback);
    matchFinder.addMatcher(returnStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}