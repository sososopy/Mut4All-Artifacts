//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_OpenMP_Directive_For_Global_Structured_Binding_394
 */ 
class MutatorFrontendAction_394 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(394)

private:
    class MutatorASTConsumer_394 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_394(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::set<std::string> globalDecompVars;
    };
};

//source file
#include "../include/Insert_OpenMP_Directive_For_Global_Structured_Binding_394.h"

// ========================================================================================================
#define MUT394_OUTPUT 1

class VarRefVisitor : public clang::RecursiveASTVisitor<VarRefVisitor> {
public:
    VarRefVisitor(const std::set<std::string>& vars) : targetVars(vars), found(false) {}
    bool VisitDeclRefExpr(clang::DeclRefExpr *E) {
        if (auto *VD = dyn_cast<clang::VarDecl>(E->getDecl())) {
            if (targetVars.count(VD->getNameAsString())) {
                found = true;
                return false;
            }
        }
        return true;
    }
    bool found;
private:
    const std::set<std::string>& targetVars;
};

void MutatorFrontendAction_394::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("decomp")) {
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;
        for (unsigned i = 0; i < DD->bindings_size(); ++i) {
            auto *VD = DD->getBinding(i);
            if (VD) {
                globalDecompVars.insert(VD->getNameAsString());
            }
        }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        bool usesDecompVar = false;
        if (FD->getBody()) {
            VarRefVisitor visitor(globalDecompVars);
            visitor.TraverseStmt(FD->getBody());
            usesDecompVar = visitor.found;
        }
        if (usesDecompVar) {
            SourceLocation loc = FD->getBody()->getBeginLoc();
            std::string pragma;
            int choice = getrandom::getRandomIndex(2);
            if (choice == 0) {
                pragma = "#pragma omp parallel for\n";
            } else {
                pragma = "#pragma omp target\n";
            }
            Rewrite.InsertText(loc, pragma, true, true);
        }
    }
}
  
void MutatorFrontendAction_394::MutatorASTConsumer_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher decompMatcher = decompositionDecl().bind("decomp");
    DeclarationMatcher funcMatcher = functionDecl().bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decompMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}