//header file
#pragma once
#include "Mutator_base.h"

/**
 * Redefine_Struct_In_Its_Own_Definition_287
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Redefine_Struct_In_Its_Own_Definition_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto name = ST->getNameAsString();
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto redefinition = "/*mut287*/struct " + name + ";\n";
      content.insert(content.find('{') + 1, redefinition);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}