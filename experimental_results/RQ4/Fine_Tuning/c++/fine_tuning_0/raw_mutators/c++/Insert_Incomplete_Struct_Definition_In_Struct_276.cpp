//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Incomplete_Struct_Definition_In_Struct_276
 */ 
class MutatorFrontendAction_276 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(276)

private:
    class MutatorASTConsumer_276 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_276(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Incomplete_Struct_Definition_In_Struct_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (!SD->isStruct())
        return;
      if (!SD->isCompleteDefinition())
        return;
      auto name = SD->getNameAsString();
      auto insertion = "struct " + name + ";";
      llvm::outs() << insertion << "\n";
      Rewrite.ReplaceText(SD->getBeginLoc(), 0, "/*mut276*/"+insertion);
    }
}
  
void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}