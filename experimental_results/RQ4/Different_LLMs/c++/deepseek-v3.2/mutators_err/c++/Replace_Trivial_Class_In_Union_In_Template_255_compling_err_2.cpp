//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Trivial_Class_In_Union_In_Template_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> nonTrivialClasses;
        std::vector<const clang::CXXRecordDecl*> trivialClassesInUnion;
        std::vector<const clang::FieldDecl*> unionMemberFields;
    };
};

//source file
#include "../include/Mutator_Replace_Trivial_Class_In_Union_In_Template_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("NonTrivialClass")) {
      //Filter nodes in header files
      if (!NT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NT->getLocation()))
        return;
      if (!NT->isCompleteDefinition())
        return;
      //Check if class is non-trivial (has user-provided constructor/destructor or virtual functions)
      bool hasUserProvidedConstructor = false;
      for (auto method : NT->methods()) {
        if (method->isUserProvided()) {
          if (method->getKind() == Decl::CXXConstructor || 
              method->getKind() == Decl::CXXDestructor) {
            hasUserProvidedConstructor = true;
            break;
          }
        }
      }
      bool hasVirtualFunction = false;
      for (auto method : NT->methods()) {
        if (method->isVirtual()) {
          hasVirtualFunction = true;
          break;
        }
      }
      if (hasVirtualFunction || hasUserProvidedConstructor) {
        //Record the node information to be used in the mutation process
        nonTrivialClasses.push_back(NT);
      }
    }
    else if (auto *TC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TrivialClassInUnion")) {
      //Filter nodes in header files
      if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TC->getLocation()))
        return;
      if (!TC->isCompleteDefinition())
        return;
      //Check if class is trivial (no user-provided constructor/destructor, no virtual functions)
      bool isTrivial = true;
      for (auto method : TC->methods()) {
        if (method->isUserProvided()) {
          if (method->getKind() == Decl::CXXConstructor || 
              method->getKind() == Decl::CXXDestructor) {
            isTrivial = false;
            break;
          }
        }
      }
      for (auto method : TC->methods()) {
        if (method->isVirtual()) {
          isTrivial = false;
          break;
        }
      }
      if (isTrivial) {
        trivialClassesInUnion.push_back(TC);
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionMemberField")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if field is inside a union within a template
      auto parent = FD->getParent();
      if (!parent || !parent->isUnion())
       1return;
      auto recordDecl = dyn_cast<CXXRecordDecl>(parent->getParent());
      if (!recordDecl || !recordDecl->getDescribedClassTemplate())
        return;
      //Record the node information to be used in the mutation process
      unionMemberFields.push_back(FD);
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
      //Perform mutation after collecting all necessary information
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      
      //Find a union member field with trivial class type to mutate
      for (auto *field : unionMemberFields) {
        auto fieldType = field->getType();
        if (fieldType->isRecordType()) {
          auto recordType = fieldType->getAsRecordDecl();
          //Check if this record type is in trivialClassesInUnion
          bool isTrivialTarget = false;
          for (auto *trivialClass : trivialClassesInUnion) {
            if (recordType == trivialClass) {
              isTrivialTarget = true;
              break;
            }
          }
          
          if (isTrivialTarget) {
            //Select a non-trivial class to replace with
            if (nonTrivialClasses.empty()) {
              //Create a new non-trivial class declaration before the union
              SourceLocation insertLoc = field->getBeginLoc();
              std::string newClassDecl = "\nstruct NonTrivial_255 { NonTrivial_255() {} };";
              Rewrite.InsertTextBefore(insertLoc, newClassDecl);
              //Get the source code text of target node
              auto fieldText = stringutils::rangetoStr(*(Result.SourceManager), field->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              //Replace the trivial class type name with NonTrivial_255
              std::string oldTypeName = trivialClassesInUnion[0]->getNameAsString();
              std::string newFieldText = fieldText;
              size_t pos = newFieldText.find(oldTypeName);
              if (pos != std::string::npos) {
                newFieldText.replace(pos, oldTypeName.length(), "NonTrivial_255");
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(field->getSourceRange()), newFieldText);
              }
            } else {
              //Use existing non-trivial class
              size_t idx = getrandom::getRandomIndex(nonTrivialClasses.size() - 1);
              auto *nonTrivialClass = nonTrivialClasses[idx];
              //Get the source code text of target node
              auto fieldText = stringutils::rangetoStr(*(Result.SourceManager), field->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              //Replace the trivial class type name with non-trivial class name
              std::string oldTypeName = trivialClassesInUnion[0]->getNameAsString();
              std::string newTypeName = nonTrivialClass->getNameAsString();
              std::string newFieldText = fieldText;
              size_t pos = newFieldText.find(oldTypeName);
              if (pos != std::string::npos) {
                newFieldText.replace(pos, oldTypeName.length(), newTypeName);
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(field->getSourceRange()), newFieldText);
              }
            }
            break; // Mutate only one field
          }
        }
      }
    }
}
  
void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher nonTrivialMatcher = cxxRecordDecl().bind("NonTrivialClass");
    DeclarationMatcher trivialMatcher = cxxRecordDecl().bind("TrivialClassInUnion");
    DeclarationMatcher unionFieldMatcher = fieldDecl(hasParent(recordDecl(isUnion(), hasParent(cxxRecordDecl(hasDescendant(templateDecl())))))).bind("UnionMemberField");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nonTrivialMatcher, &callback);
    matchFinder.addMatcher(trivialMatcher, &callback);
    matchFinder.addMatcher(unionFieldMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}