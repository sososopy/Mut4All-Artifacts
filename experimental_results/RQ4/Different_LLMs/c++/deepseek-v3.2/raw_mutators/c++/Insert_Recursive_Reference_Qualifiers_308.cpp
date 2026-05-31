//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Reference_Qualifiers_308
 */ 
class MutatorFrontendAction_308 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(308)

private:
    class MutatorASTConsumer_308 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_308(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (!type->isReferenceType())
        return;
      auto sourceRange = VD->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string newTypeText = "";
      auto qualifiers = type.getQualifiers();
      bool hasConst = qualifiers.hasConst();
      bool hasVolatile = qualifiers.hasVolatile();
      int depth = getrandom::getRandomIndex(15) + 5; // 5 to 20 layers
      for (int i = 0; i < depth; ++i) {
          if (i % 2 == 0) {
              newTypeText += "const ";
              if (hasVolatile)
                  newTypeText += "volatile ";
          } else {
              if (type->isLValueReferenceType())
                  newTypeText += "& ";
              else if (type->isRValueReferenceType())
                  newTypeText += "&& ";
          }
      }
      //Replace the original AST node with the mutated one
      std::string mutatedText = newTypeText + sourceText;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(referenceType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}