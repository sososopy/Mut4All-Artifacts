//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Friend_Declaration_133
 */ 
class MutatorFrontendAction_133 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(133)

private:
    class MutatorASTConsumer_133 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_133(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingConcepts;
        std::vector<std::string> existingTemplateClasses;
    };
};

//source file
#include "../include/Mutator_133.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("FriendDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto friendDeclText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Check if friend declaration is templated and uses a concept
      if (auto *FTD = FD->getFriendType()) {
        if (auto *TST = dyn_cast<TemplateSpecializationType>(FTD->getType())) {
          //Found a friend declaration with template specialization
          //We'll mutate it by changing the constraint or adding another friend
          int choice = getrandom::getRandomIndex(1); // 0: change constraint, 1: add another friend
          
          if (choice == 0 && !existingConcepts.empty()) {
            //Change the constraint to a different concept
            std::string newConcept = existingConcepts[getrandom::getRandomIndex(existingConcepts.size()-1)];
            std::string oldConcept = "";
            //Extract old concept name (simplified parsing)
            size_t templatePos = friendDeclText.find("template <");
            if (templatePos != std::string::npos) {
              size_t conceptPos = friendDeclText.find("concept", templatePos);
              if (conceptPos != std::string::npos) {
                //Find concept name
                size_t start = friendDeclText.find_first_not_of(" \t", conceptPos + 7);
                size_t end = friendDeclText.find_first_of(" \t<", start);
                oldConcept = friendDeclText.substr(start, end-start);
              }
            }
            if (!oldConcept.empty() && oldConcept != newConcept) {
              //Replace old concept with new concept
              std::string mutatedText = friendDeclText;
              size_t pos = mutatedText.find(oldConcept);
              while (pos != std::string::npos) {
                mutatedText.replace(pos, oldConcept.length(), newConcept);
                pos = mutatedText.find(oldConcept, pos + newConcept.length());
              }
              mutatedText = "/*mut133*/" + mutatedText;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
            }
          } else if (choice == 1 && !existingTemplateClasses.empty()) {
            //Add another friend declaration with same/different constraint
            std::string newFriend = "\n\t/*mut133*/template <";
            if (!existingConcepts.empty()) {
              std::string concept = existingConcepts[getrandom::getRandomIndex(existingConcepts.size()-1)];
              newFriend += concept + " V> friend class ";
            } else {
              newFriend += "typename V> friend class ";
            }
            std::string className = existingTemplateClasses[getrandom::getRandomIndex(existingTemplateClasses.size()-1)];
            newFriend += className + ";";
            
            //Insert after current friend declaration
            SourceLocation insertLoc = FD->getEndLoc();
            Rewrite.InsertTextAfterToken(insertLoc, newFriend);
          }
        }
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Record existing concepts
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      existingConcepts.push_back(CD->getNameAsString());
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplateDecl")) {
      //Record existing template classes
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      existingTemplateClasses.push_back(CTD->getNameAsString());
    }
}
  
void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher friendMatcher = friendDecl().bind("FriendDecl");
    DeclarationMatcher conceptMatcher = conceptDecl(isExpansionInMainFile()).bind("ConceptDecl");
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}