//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Malformed_Member_After_Template_Dependent_340
 */ 
class MutatorFrontendAction_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(340)

private:
    class MutatorASTConsumer_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_340(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Inject_Malformed_Member_After_Template_Dependent_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("templateDependentField")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if field type involves template specialization or decltype
      auto QT = FD->getType();
      if (QT.isNull()) return;
      
      bool isTemplateSpecialization = false;
      bool isDecltype = false;
      
      // Check for template specialization
      if (QT->getAs<clang::TemplateSpecializationType>()) {
        isTemplateSpecialization = true;
      }
      // Check for decltype
      else if (QT->getAs<clang::DecltypeType>()) {
        isDecltype = true;
      }
      // Check for elaborated type (like SomeTemplate<int>)
      else if (auto *ET = QT->getAs<clang::ElaboratedType>()) {
        auto *NT = ET->getNamedType()->getAs<clang::TemplateSpecializationType>();
        if (NT) isTemplateSpecialization = true;
      }
      
      if (!isTemplateSpecialization && !isDecltype) return;
      
      // Record candidate field
      candidateFields.push_back(FD);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithCandidate")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      
      if (!CD->isCompleteDefinition()) return;
      
      // Find candidate fields within this class
      std::vector<const clang::FieldDecl*> fieldsInThisClass;
      for (auto FD : candidateFields) {
        if (FD->getParent() == CD) {
          fieldsInThisClass.push_back(FD);
        }
      }
      
      if (fieldsInThisClass.empty()) return;
      
      // Randomly select one candidate field
      size_t idx = getrandom::getRandomIndex(fieldsInThisClass.size() - 1);
      const clang::FieldDecl* targetField = fieldsInThisClass[idx];
      
      // Get source location after the field declaration
      SourceLocation insertLoc = targetField->getEndLoc();
      // Move past the semicolon
      insertLoc = Rewrite.getSourceMgr().getLocForEndOfFile(insertLoc);
      
      // Choose identifier: either reuse existing identifier from class or use 'x'
      std::string identifier = "x";
      // Look for any identifier in class members that is a single lowercase letter
      for (auto decl : CD->decls()) {
        if (auto *ND = dyn_cast<clang::NamedDecl>(decl)) {
          std::string name = ND->getNameAsString();
          if (name.length() == 1 && name[0] >= 'a' && name[0] <= 'z') {
            identifier = name;
            break;
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string malformedLine = "\n    " + identifier + "   // injected syntax error";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, malformedLine);
      
      // Clear candidate fields for this class to avoid duplicate mutations
      candidateFields.clear();
    }
}
  
void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher fieldMatcher = fieldDecl(hasType(anyOf(
        templateSpecializationType(),
        decltypeType(),
        elaboratedType(hasUnqualifiedDesugaredType(templateSpecializationType()))
    ))).bind("templateDependentField");
    
    DeclarationMatcher classMatcher = cxxRecordDecl(isClass()).bind("classWithCandidate");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}