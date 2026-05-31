//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Syntax_340
 */ 
class MutatorFrontendAction_Insert_Invalid_Syntax_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Insert_Invalid_Syntax_340)

private:
    class MutatorASTConsumer_Insert_Invalid_Syntax_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Invalid_Syntax_340(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Insert_Invalid_Syntax_340.h"
#include "llvm/ADT/StringRef.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_Insert_Invalid_Syntax_340::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange()));
      //Perform mutation on the source code text by applying string replacement
      // Insert a random keyword or symbol at a random location within the class or struct body
      int insertLocation = getrandom::getRandomIndex(declaration.size());
      std::string invalidSyntax = "p"; // Example of a random keyword
      std::string mutatedDeclaration = declaration.str();
      mutatedDeclaration = mutatedDeclaration.substr(0, insertLocation) + invalidSyntax + mutatedDeclaration.substr(insertLocation);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_Insert_Invalid_Syntax_340::MutatorASTConsumer_Insert_Invalid_Syntax_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("CXXRecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}