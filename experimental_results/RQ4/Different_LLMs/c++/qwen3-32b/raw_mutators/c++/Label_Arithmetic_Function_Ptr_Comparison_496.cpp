//header file
#pragma once
#include "Mutator_base.h"

/**
 * Label_Arithmetic_Function_Ptr_Comparison_496
 */ 
class MutatorFrontendAction_496 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(496)
private:
    class MutatorASTConsumer_496 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_496(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Label_Arithmetic_Function_Ptr_Comparison_496.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT496_OUTPUT 1

void MutatorFrontendAction_496::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody()) return;
        auto Body = FD->getBody();
        if (!Body) return;
        
        SmallVector<clang::LabelStmt*, 4> Labels;
        for (const auto *S : Body->body()) {
            if (auto *LS = clang::dyn_cast<clang::LabelStmt>(S)) {
                Labels.push_back(LS);
            }
        }
        if (Labels.size() < 2) return;
        
        auto Label1 = Labels[0]->getLabel()->getNameAsString();
        auto Label2 = Labels[1]->getLabel()->getNameAsString();
        auto FuncName = FD->getNameAsString();
        
        std::string Code = "if (&&" + Label1 + " - &&" + Label2 + " < " + FuncName + ") {}";
        SourceLocation EndLoc = Body->getRBracLoc();
        Rewrite.InsertText(EndLoc, "\n" + Code, true, true);
    }
}
  
void MutatorFrontendAction_496::MutatorASTConsumer_496::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}