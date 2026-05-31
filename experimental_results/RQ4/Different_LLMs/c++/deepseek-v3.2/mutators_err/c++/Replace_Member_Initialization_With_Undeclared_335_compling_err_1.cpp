//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Initialization_With_Undeclared_335
 */ 
class MutatorFrontendAction_335 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(335)

private:
    class MutatorASTConsumer_335 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_335(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> visitedClasses;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->hasBody())
        return;
      
      const clang::CXXRecordDecl* classDecl = MT->getParent();
      if (!classDecl || !classDecl->isCompleteDefinition())
        return;
      
      // Record class for member checking
      visitedClasses.push_back(classDecl);
      
      // Check if constructor has initializer list
      if (!MT->getNumCtorInitializers() > 0)
        return;
      
      // Select a random initializer to mutate
      int initIndex = getrandom::getRandomIndex(MT->getNumCtorInitializers() - 1);
      const clang::CXXCtorInitializer* init = MT->getInit(initIndex);
      
      if (!init || !init->isMemberInitializer())
        return;
      
      const clang::FieldDecl* member = init->getMember();
      if (!member)
        return;
      
      std::string memberName = member->getNameAsString();
      
      // Generate undeclared member name
      std::string undeclaredName = "undeclared_member";
      // Ensure it's not an existing member
      for (const clang::FieldDecl* field : classDecl->fields()) {
        if (field->getNameAsString() == undeclaredName) {
          undeclaredName = "undeclared_member_" + std::to_string(getrandom::getRandomIndex(1000));
          break;
        }
      }
      
      // Get source range of the initializer
      SourceRange initRange = init->getSourceRange();
      if (initRange.isInvalid())
        return;
      
      std::string initText = stringutils::rangetoStr(*(Result.SourceManager), initRange);
      
      // Perform mutation: replace member name with undeclared name
      // Find position of member name in initializer text
      size_t pos = initText.find(memberName);
      if (pos == std::string::npos)
        return;
      
      std::string mutatedText = initText.substr(0, pos) + undeclaredName + initText.substr(pos + memberName.length());
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange), mutatedText);
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}