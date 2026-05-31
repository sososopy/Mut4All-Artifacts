//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Transparent_Union_Attribute_151
 */ 
class MutatorFrontendAction_151 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(151)

private:
    class MutatorASTConsumer_151 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_151(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Transparent_Union_Attribute_151.h"

// ========================================================================================================
#define MUT151_OUTPUT 1

void MutatorFrontendAction_151::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("UnionDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      // Ensure it's a union and not already having transparent_union attribute
      if (!UD->isUnion() || UD->hasAttr<clang::TransparentUnionAttr>())
        return;
      // Ensure it's at file or namespace scope, not within a class or function
      if (UD->getParentFunctionOrMethod() || UD->getParent()->isRecord())
        return;
      //Get the source code text of target node
      auto SourceRange = UD->getSourceRange();
      if (SourceRange.isInvalid())
        return;
      std::string unionText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      // Check if it's a typedef union
      bool isTypedef = false;
      if (auto *TND = dyn_cast<clang::TypedefNameDecl>(UD)) {
        isTypedef = true;
      }
      // Find the end location to insert attribute
      size_t insertPos = unionText.length();
      // Remove trailing semicolon if present, attribute will be added before it
      if (unionText.back() == ';') {
        insertPos--;
      }
      // Insert the attribute
      std::string attribute = " __attribute__ ((__transparent_union__))";
      unionText.insert(insertPos, attribute);
      // Add back semicolon if it was removed
      if (unionText.back() != ';') {
        unionText += ';';
      }
      // Add mutation comment
      unionText = "/*mut151*/" + unionText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), unionText);
    }
}
  
void MutatorFrontendAction_151::MutatorASTConsumer_151::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isUnion()).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}