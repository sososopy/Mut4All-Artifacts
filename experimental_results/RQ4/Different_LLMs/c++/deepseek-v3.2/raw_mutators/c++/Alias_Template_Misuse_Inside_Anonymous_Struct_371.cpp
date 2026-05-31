//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alias_Template_Misuse_Inside_Anonymous_Struct_371
 */ 
class MutatorFrontendAction_371 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(371)

private:
    class MutatorASTConsumer_371 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_371(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasTemplateDecl*> aliasTemplates;
        std::vector<const clang::FieldDecl*> candidateFields;
    };
};

//source file
#include "../include/Alias_Template_Misuse_Inside_Anonymous_Struct_371.h"

// ========================================================================================================
#define MUT371_OUTPUT 1

void MutatorFrontendAction_371::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      aliasTemplates.push_back(AT);
    }
    //Check whether the matched AST node is the target node
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      candidateFields.push_back(FD);
    }
    //Check whether the matched AST node is the target node
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      if (aliasTemplates.empty() || candidateFields.empty())
        return;
      size_t aliasIdx = getrandom::getRandomIndex(aliasTemplates.size() - 1);
      size_t fieldIdx = getrandom::getRandomIndex(candidateFields.size() - 1);
      const clang::TypeAliasTemplateDecl* aliasDecl = aliasTemplates[aliasIdx];
      const clang::FieldDecl* fieldDecl = candidateFields[fieldIdx];
      if (!aliasDecl || !fieldDecl)
        return;
      //Check if the field belongs to this record
      const clang::CXXRecordDecl* fieldRecord = llvm::dyn_cast<clang::CXXRecordDecl>(fieldDecl->getParent());
      if (!fieldRecord || fieldRecord != RD)
        return;
      string aliasName = aliasDecl->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      string mutatedText = aliasName + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(fieldDecl->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_371::MutatorASTConsumer_371::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("AliasTemplate");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}