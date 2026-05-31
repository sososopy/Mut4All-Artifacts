//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Redefinition_287
 */ 
class MutatorFrontendAction_287 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(287)

private:
    class MutatorASTConsumer_287 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_287(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Redefinition_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (!SD->isStruct() || !SD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto structName = SD->getNameAsString();
      if (structName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string redefinedStruct = "\nstruct " + structName + " { float y; };\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfter(SD->getEndLoc(), "/*mut287*/" + redefinedStruct);
    }
}
  
void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}