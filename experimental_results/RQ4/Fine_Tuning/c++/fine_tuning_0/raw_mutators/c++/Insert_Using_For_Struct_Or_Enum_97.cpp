//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_For_Struct_Or_Enum_97
 */ 
class MutatorFrontendAction_97 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(97)

private:
    class MutatorASTConsumer_97 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_97(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Using_For_Struct_Or_Enum_97.h"

// ========================================================================================================
#define MUT97_OUTPUT 1

void MutatorFrontendAction_97::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TagDecl>("StructOrEnum")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (DL->isStruct() || DL->isEnum()) {
        auto name = DL->getNameAsString();
        auto using_decl = "/*mut97*/ using " + name + " :: ;\n";
        Rewrite.ReplaceText(DL->getEndLoc(), 0, using_decl);
      }
    }
}
  
void MutatorFrontendAction_97::MutatorASTConsumer_97::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = tagDecl().bind("StructOrEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}