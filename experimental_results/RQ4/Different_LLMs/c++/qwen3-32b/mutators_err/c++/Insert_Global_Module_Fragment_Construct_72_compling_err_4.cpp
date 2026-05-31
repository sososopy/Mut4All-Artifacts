//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Basic/Module.h"

/**
 * Insert_Global_Module_Fragment_Construct_72
 */ 
class MutatorFrontendAction_Insert_Global_Module_Fragment_Construct_72 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Insert_Global_Module_Fragment_Construct_72)
private:
    class MutatorASTConsumer_Insert_Global_Module_Fragment_Construct_72 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Global_Module_Fragment_Construct_72(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Global_Module_Fragment_Construct_72.h"

// ========================================================================================================
#define MUTInsert_Global_Module_Fragment_Construct_72_OUTPUT 1

void MutatorFrontendAction_Insert_Global_Module_Fragment_Construct_72::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::ModuleDecl>("moduleDecl")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;
        SourceLocation insertLoc = MD->getLocation();
        Rewrite.InsertTextBefore(insertLoc, "global module fragment;\n");
    }
}
  
void MutatorFrontendAction_Insert_Global_Module_Fragment_Construct_72::MutatorASTConsumer_Insert_Global_Module_Fragment_Construct_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = moduleDecl(unless(isGlobalModuleFragment())).bind("moduleDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}