//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Parameter_In_Requires_Clause_119
 */ 
class MutatorFrontendAction_119 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(119)
private:
    class MutatorASTConsumer_119 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_119(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Function_Template_Parameter_In_Requires_Clause_119.h"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *RC = FD->getRequiresClause()) {
            auto *RE = RC->getExpr();
            if (!RE)
                return;

            std::vector<const clang::ParmVarDecl*> params;
            for (const auto *param : FD->parameters()) {
                params.push_back(param);
            }

            if (params.size() < 2)
                return;

            const clang::ParmVarDecl *param1 = params[0];
            const clang::ParmVarDecl *param2 = params[1];

            std::string name1 = param1->getNameAsString();
            std::string name2 = param2->getNameAsString();

            SourceManager &SM = *Result.SourceManager;
            SourceRange exprRange = RE->getSourceRange();
            std::string exprText = stringutils::rangetoStr(SM, exprRange);

            std::string mutatedExpr = exprText;
            size_t pos = 0;
            while ((pos = mutatedExpr.find(name1, pos)) != std::string::npos) {
                mutatedExpr.replace(pos, name1.length(), name2);
                pos += name2.length();
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(exprRange), mutatedExpr);
        }
    }
}
  
void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isFunctionTemplate(), hasRequiresClause()).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}