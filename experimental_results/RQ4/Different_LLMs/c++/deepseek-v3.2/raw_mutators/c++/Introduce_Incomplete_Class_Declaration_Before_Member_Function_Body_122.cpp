//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Class_Declaration_Before_Member_Function_Body_122
 */ 
class MutatorFrontendAction_122 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(122)

private:
    class MutatorASTConsumer_122 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_122(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> existingTypeNames;
        std::string generateUniqueTypeName();
    };
};

//source file
#include "../include/Mutator_Introduce_Incomplete_Class_Declaration_Before_Member_Function_Body_122.h"

// ========================================================================================================
#define MUT122_OUTPUT 1

std::string MutatorFrontendAction_122::Callback::generateUniqueTypeName() {
    std::string baseName = "IncompleteClass";
    std::string candidate = baseName;
    int suffix = -1;
    while (existingTypeNames.find(candidate) != existingTypeNames.end()) {
        suffix++;
        candidate = baseName + std::to_string(suffix);
    }
    existingTypeNames.insert(candidate);
    return candidate;
}

void MutatorFrontendAction_122::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->hasBody())
        return;
      auto *Body = MT->getBody();
      if (!Body)
        return;
      //Check if the parent class has a member without a type specifier
      auto *Parent = dyn_cast<CXXRecordDecl>(MT->getParent());
      if (!Parent)
        return;
      bool hasMissingTypeMember = false;
      for (auto *Field : Parent->fields()) {
          if (!Field->getTypeSourceInfo() || Field->getTypeSourceInfo()->getType().isNull()) {
              hasMissingTypeMember = true;
              break;
          }
      }
      if (!hasMissingTypeMember)
          return;
      //Collect existing type names in the translation unit
      auto &Context = *Result.Context;
      for (auto *Decl : Context.getTranslationUnitDecl()->decls()) {
          if (auto *TypeDecl = dyn_cast<TypeDecl>(Decl)) {
              if (TypeDecl->getIdentifier())
                  existingTypeNames.insert(TypeDecl->getNameAsString());
          }
      }
      //Perform mutation on the source code text by applying string replacement
      std::string incompleteDecl = "class " + generateUniqueTypeName() + ";";
      SourceLocation InsertLoc = Body->getBeginLoc().getLocWithOffset(1); // After '{'
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(InsertLoc, "\n\t" + incompleteDecl + "\n");
    }
}
  
void MutatorFrontendAction_122::MutatorASTConsumer_122::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(
        hasParent(cxxRecordDecl(has(fieldDecl(unless(hasType(type()))))))
    ).bind("MemberFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}