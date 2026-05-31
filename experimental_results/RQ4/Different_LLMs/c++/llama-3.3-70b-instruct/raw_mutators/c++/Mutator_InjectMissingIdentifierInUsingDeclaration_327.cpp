//header file
#pragma once
#include "Mutator_base.h"

/**
 * InjectMissingIdentifierInUsingDeclaration_327
 */ 
class MutatorFrontendAction_InjectMissingIdentifierInUsingDeclaration_327 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(InjectMissingIdentifierInUsingDeclaration_327)

private:
    class MutatorASTConsumer_InjectMissingIdentifierInUsingDeclaration_327 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_InjectMissingIdentifierInUsingDeclaration_327(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_InjectMissingIdentifierInUsingDeclaration_327.h"

// ========================================================================================================
#define MUT_InjectMissingIdentifierInUsingDeclaration_327_OUTPUT 1

void MutatorFrontendAction_InjectMissingIdentifierInUsingDeclaration_327::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getQualifierLoc().isValid()) {
        std::string mutatedDeclaration = declaration;
        if (declaration.find("::") != std::string::npos) {
          size_t pos = declaration.find("::");
          mutatedDeclaration.insert(pos + 2, "foo");
        } else {
          mutatedDeclaration.insert(0, "MyClass::");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_InjectMissingIdentifierInUsingDeclaration_327::MutatorASTConsumer_InjectMissingIdentifierInUsingDeclaration_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl().bind("UsingDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}