//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_initialization_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        const clang::VarDecl *unionVarDecl;
    };
};

//source file
#include "../include/modify_union_initialization_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("unionVar")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        VD->getLocation()))
            return;

        if (const auto *InitList = llvm::dyn_cast<clang::InitListExpr>(VD->getInit())) {
            if (InitList->getNumInits() == 1) {
                //Get the source code text of target node
                auto initText = stringutils::rangetoStr(*(Result.SourceManager), InitList->getSourceRange());
                //Perform mutation on the source code text by applying string replacement
                std::string mutatedInit = "{.x = 10, .y = 2.5f}";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitList->getSourceRange()), mutatedInit);
            }
        }
    }
}
  
void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(unionType(hasDeclaration(recordDecl(isUnion())))), hasInitializer(initListExpr().bind("initList"))).bind("unionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}