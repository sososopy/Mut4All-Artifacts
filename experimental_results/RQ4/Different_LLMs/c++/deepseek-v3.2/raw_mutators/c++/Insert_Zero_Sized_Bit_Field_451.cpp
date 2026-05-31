//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Zero_Sized_Bit_Field_451
 */ 
class MutatorFrontendAction_451 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(451)

private:
    class MutatorASTConsumer_451 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_451(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FieldDecl*> candidateFields;
    };
};

//source file
#include "../include/Insert_Zero_Sized_Bit_Field_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate fields
      if (!FD->isBitField() || FD->getBitWidthValue(Result.Context) != 0) {
        candidateFields.push_back(FD);
      }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      if (candidateFields.empty())
        return;

      //Select a field to insert after
      size_t index = getrandom::getRandomIndex(candidateFields.size() - 1);
      const clang::FieldDecl* targetField = candidateFields[index];
      if (!targetField->getParent()->equals(RD))
        return;

      //Determine bit-field type
      std::string bitFieldType = "unsigned";
      clang::QualType fieldType = targetField->getType();
      if (fieldType->isIntegralType(Result.Context) || fieldType->isEnumeralType()) {
        bitFieldType = fieldType.getAsString();
      }

      //Get insertion location
      clang::SourceLocation insertLoc = targetField->getEndLoc();
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n\t" + bitFieldType + " : 0;";
      insertion = "/*mut451*/" + insertion;
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, insertion);
      candidateFields.clear();
    }
}
  
void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("CXXRecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}