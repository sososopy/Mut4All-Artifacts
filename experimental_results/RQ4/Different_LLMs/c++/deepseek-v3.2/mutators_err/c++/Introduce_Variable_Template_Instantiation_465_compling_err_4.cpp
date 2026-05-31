//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Variable_Template_Instantiation_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarTemplateDecl*> variableTemplates;
        std::vector<const clang::ClassTemplateDecl*> templateClasses;
        std::vector<const clang::VarDecl*> variableInstances;
    };
};

//source file
#include "../include/Introduce_Variable_Template_Instantiation_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
      //Filter nodes in header files
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VT->getLocation()))
        return;
      //Record variable template declarations
      variableTemplates.push_back(VT);
    }
    else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Record template class declarations
      templateClasses.push_back(CT);
    }
    else if (auto *VI = Result.Nodes.getNodeAs<clang::VarDecl>("VarInstance")) {
      //Filter nodes in header files
      if (!VI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VI->getLocation()))
        return;
      //Check if this is a variable template specialization
      if (VI->getDescribedVarTemplate() || VI->getTemplateSpecializationKind() != clang::TSK_Undeclared) {
        //Record variable template instances
        variableInstances.push_back(VI);
      }
    }
    else if (auto *VI = Result.Nodes.getNodeAs<clang::VarDecl>("VarInstanceForMutation")) {
      //Filter nodes in header files
      if (!VI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VI->getLocation()))
        return;
      //Get the source code text of target node
      auto sourceRange = VI->getSourceRange();
      if (!sourceRange.isValid()) return;
      
      //Check if we have suitable template classes with static constexpr members
      std::vector<const clang::VarDecl*> candidateMembers;
      for (auto CT : templateClasses) {
        if (!CT->getTemplatedDecl()) continue;
        auto recordDecl = dyn_cast<clang::CXXRecordDecl>(CT->getTemplatedDecl());
        if (!recordDecl) continue;
        
        //Look for static constexpr members
        for (auto decl : recordDecl->decls()) {
          if (auto var = dyn_cast<clang::VarDecl>(decl)) {
            if (var->isStaticDataMember() && var->isConstexpr()) {
              //Check if template class is instantiated somewhere
              bool hasInstantiation = false;
              for (auto spec : CT->specializations()) {
                if (spec->getTemplateSpecializationKind() != clang::TSK_Undeclared) {
                  hasInstantiation = true;
                  break;
                }
              }
              if (hasInstantiation) {
                candidateMembers.push_back(var);
              }
            }
          }
        }
      }
      
      if (candidateMembers.empty()) {
        //If no suitable template class exists, create a new variable template
        if (variableTemplates.empty()) {
          //Create a new variable template declaration before this variable
          std::string newTemplate = "\n/*mut465*/template<typename... Args>\nconstexpr int VariableTemplate_465 = 0;\n";
          Rewrite.InsertTextBefore(VI->getBeginLoc(), newTemplate);
          return;
        }
        //Use existing variable template but need to modify its arguments
        return;
      }
      
      //Select a random candidate member
      size_t memberIdx = getrandom::getRandomIndex(candidateMembers.size() - 1);
      auto member = candidateMembers[memberIdx];
      auto memberParent = member->getDeclContext();
      
      //Find the template class that contains this member
      const clang::ClassTemplateDecl* containingTemplate = nullptr;
      for (auto CT : templateClasses) {
        if (CT->getTemplatedDecl() == memberParent) {
          containingTemplate = CT;
          break;
        }
      }
      
      if (!containingTemplate) return;
      
      //Find a template instantiation of this class
      std::string templateInstanceName;
      for (auto spec : containingTemplate->specializations()) {
        if (spec->getTemplateSpecializationKind() != clang::TSK_Undeclared) {
          templateInstanceName = spec->getNameAsString();
          break;
        }
      }
      
      if (templateInstanceName.empty()) return;
      
      //Get the original source code
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Perform mutation on the source code text by applying string replacement
      //Replace or add template argument with static constexpr member
      std::string mutatedText;
      
      //Check if this is a variable template specialization
      if (VI->getDescribedVarTemplate() || VI->getTemplateSpecializationKind() != clang::TSK_Undeclared) {
        //Find the template arguments
        if (auto templateSpecialization = dyn_cast<clang::VarTemplateSpecializationDecl>(VI)) {
          //We'll insert the member reference as a new template argument
          mutatedText = originalText;
          //Find the template argument list
          size_t templateNameEnd = mutatedText.find('<');
          if (templateNameEnd != std::string::npos) {
            size_t argStart = templateNameEnd + 1;
            std::string memberRef = templateInstanceName + "::" + member->getNameAsString();
            //Insert the member reference as first argument
            mutatedText.insert(argStart, memberRef + ", ");
          } else {
            //If no template arguments, add them
            size_t nameEnd = mutatedText.find(' ');
            if (nameEnd != std::string::npos) {
              std::string memberRef = templateInstanceName + "::" + member->getNameAs2String();
              mutatedText.insert(nameEnd, "<" + memberRef + ">");
            }
          }
        } else {
          //Regular variable declaration - convert to template specialization
          size_t nameEnd = originalText.find('=');
          if (nameEnd != std::string::npos) {
            std::string varName = originalText.substr(0, nameEnd);
            std::string memberRef = templateInstanceName + "::" + member->getNameAsString();
            mutatedText = varName + "<" + memberRef + ">" + originalText.substr(nameEnd);
          }
        }
      } else {
        //Not a template specialization - we need to make it one
        //Find an existing variable template to use
        if (!variableTemplates.empty()) {
          size_t templateIdx = getrandom::getRandomIndex(variableTemplates.size() - 1);
          auto varTemplate = variableTemplates[templateIdx];
          std::string templateName = varTemplate->getNameAsString();
          
          //Replace variable name with template specialization
          size_t nameEnd = originalText.find('=');
          if (nameEnd != std::string::npos) {
            std::string varName = originalText.substr(0, nameEnd);
            std::string memberRef = templateInstanceName + "::" + member->getNameAsString();
            mutatedText = templateName + "<" + memberRef + ">" + originalText.substr(nameEnd);
          }
        }
      }
      
      mutatedText = "/*mut465*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varTemplateMatcher = varTemplateDecl().bind("VarTemplate");
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher varInstanceMatcher = varDecl().bind("VarInstance");
    DeclarationMatcher varInstanceForMutationMatcher = varDecl(isExpansionInMainFile()).bind("VarInstanceForMutation");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varTemplateMatcher, &callback);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(varInstanceMatcher, &callback);
    matchFinder.addMatcher(varInstanceForMutationMatcher, &callback);
    matchFinder.matchAST(Context);
}