//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Switch_Lambda_Template_Case_436
 */ 
class MutatorFrontendAction_436 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(436)

private:
    class MutatorASTConsumer_436 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_436(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::SwitchStmt*> switches;
        std::vector<const clang::VarDecl*> lambdas;
    };
};

//source file
#include "../include/Insert_Switch_Lambda_Template_Case_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *switchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("switchStmt")) {
        if (!switchStmt || !Result.Context->getSourceManager().isWrittenInMainFile(switchStmt->getBeginLoc()))
            return;
        switches.push_back(switchStmt);
    } else if (auto *lambdaVar = Result.Nodes.getNodeAs<clang::VarDecl>("lambdaVar")) {
        if (!lambdaVar || !Result.Context->getSourceManager().isWrittenInMainFile(lambdaVar->getBeginLoc()))
            return;
        lambdas.push_back(lambdaVar);
    }

    for (const auto *sw : switches) {
        for (const auto *ld : lambdas) {
            const auto *swFunc = clang::dyn_cast<clang::FunctionDecl>(sw->getAncestor(clang::ASTNodeKind::getFromPtr(sw)));
            const auto *ldFunc = clang::dyn_cast<clang::FunctionDecl>(ld->getAncestor(clang::ASTNodeKind::getFromPtr(ld)));
            if (!swFunc || !ldFunc || swFunc != ldFunc)
                continue;

            std::string lambdaName = ld->getNameAsString();
            if (lambdaName.empty())
                continue;

            std::string caseCode = "case 3: {\n    " + lambdaName + ".template operator()<3>();\n}\n";
            SourceLocation insertLoc = sw->getLocStart();
            Rewrite.InsertText(insertLoc, caseCode);
            break;
        }
    }
}

void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher switchMatcher = switchStmt().bind("switchStmt");
    DeclarationMatcher lambdaMatcher = varDecl(hasInitializer(lambdaExpr(hasBody(cxxMethodDecl(isTemplate(), hasName("operator()")))))).bind("lambdaVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(switchMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}