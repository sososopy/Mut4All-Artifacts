//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Forward_Declaration_Bool_To_Int_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Enum_Forward_Declaration_Bool_To_Int_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("enumForwardDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->isThisDeclarationADefinition() && MT->isFixed()) {
        QualType underlyingType = MT->getIntegerType();
        if (underlyingType->isBooleanType()) {
          SourceRange range = MT->getSourceRange();
          std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
          //Perform mutation on the source code text by applying string replacement
          size_t colonPos = originalText.find(":");
          if (colonPos != std::string::npos) {
            std::string mutatedText = originalText.substr(0, colonPos + 1) + " int" + originalText.substr(colonPos + 4);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(range, mutatedText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isExpansionInMainFile(), unless(hasDefinition())).bind("enumForwardDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}