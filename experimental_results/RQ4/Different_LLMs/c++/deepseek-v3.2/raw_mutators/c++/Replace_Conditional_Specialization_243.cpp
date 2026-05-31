//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Conditional_Specialization_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(CTD);
    }
    else if (auto *CTS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!CTS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTS->getLocation()))
        return;
      if (!CTS->isExplicitSpecialization())
        return;
      
      //Get the source code text of target node
      auto specializationRange = CTS->getSourceRange();
      if (specializationRange.isInvalid())
        return;
      
      std::string specializationText = stringutils::rangetoStr(*(Result.SourceManager), specializationRange);
      
      //Find the corresponding template declaration
      const clang::ClassTemplateDecl *templateDecl = CTS->getSpecializedTemplate();
      if (!templateDecl)
        return;
      
      //Check if this specialization is conditional (has non-type template parameter)
      const clang::TemplateArgumentList &args = CTS->getTemplateArgs();
      bool hasNonTypeArg = false;
      for (unsigned i = 0; i < args.size(); ++i) {
        if (args[i].getKind() == clang::TemplateArgument::Integral) {
          hasNonTypeArg = true;
          break;
        }
      }
      
      if (!hasNonTypeArg)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //Replace boolean/integral condition with trait access
      std::string mutatedText = specializationText;
      
      //Find the template parameters in the specialization
      size_t templateStart = mutatedText.find("template<");
      if (templateStart == std::string::npos)
        return;
      
      size_t templateEnd = mutatedText.find(">", templateStart);
      if (templateEnd == std::string::npos)
        return;
      
      //Extract template parameter list
      std::string templateParams = mutatedText.substr(templateStart, templateEnd - templateStart + 1);
      
      //Check if there's a boolean/integral parameter
      if (templateParams.find("bool") == std::string::npos && 
          templateParams.find("int") == std::string::npos &&
          templateParams.find("unsigned") == std::string::npos &&
          templateParams.find("short") == std::string::npos &&
          templateParams.find("long") == std::string::npos &&
          templateParams.find("char") == std::string::npos)
        return;
      
      //Replace the parameter with a trait-based parameter
      std::string newParams = templateParams;
      
      //Find a type parameter to use for trait
      std::string typeParam = "";
      size_t typePos = newParams.find("typename");
      if (typePos != std::string::npos) {
        size_t typeEnd = newParams.find(",", typePos);
        if (typeEnd == std::string::npos) typeEnd = newParams.find(">", typePos);
        if (typeEnd != std::string::npos) {
          std::string afterType = newParams.substr(typePos, typeEnd - typePos);
          size_t nameStart = afterType.find_last_of(" ");
          if (nameStart != std::string::npos) {
            typeParam = afterType.substr(nameStart + 1);
            //Remove any trailing spaces or commas
            while (typeParam.back() == ' ' || typeParam.back() == ',')
              typeParam.pop_back();
          }
        }
      }
      
      if (typeParam.empty()) {
        //Look for class parameter
        size_t classPos = newParams.find("class");
        if (classPos != std::string::npos) {
          size_t classEnd = newParams.find(",", classPos);
          if (classEnd == std::string::npos) classEnd = newParams.find(">", classPos);
          if (classEnd != std::string::npos) {
            std::string afterClass = newParams.substr(classPos, classEnd - classPos);
            size_t nameStart = afterClass.find_last_of(" ");
            if (nameStart != std::string::npos) {
              typeParam = afterClass.substr(nameStart + 1);
              while (typeParam.back() == ' ' || typeParam.back() == ',')
                typeParam.pop_back();
            }
          }
        }
      }
      
      if (typeParam.empty())
        return;
      
      //Replace the non-type parameter with trait access
      size_t boolPos = newParams.find("bool");
      if (boolPos != std::string::npos) {
        size_t boolEnd = newParams.find(",", boolPos);
        if (boolEnd == std::string::npos) boolEnd = newParams.find(">", boolPos);
        if (boolEnd != std::string::npos) {
          std::string boolParam = newParams.substr(boolPos, boolEnd - boolPos);
          //Replace with trait access
          std::string traitAccess = "typename Trait<" + typeParam + ">::value";
          newParams.replace(boolPos, boolParam.length(), traitAccess);
        }
      } else {
        //Try other integral types
        std::vector<std::string> integralTypes = {"int", "unsigned", "short", "long", "char"};
        for (const auto &type : integralTypes) {
          size_t typePos = newParams.find(type);
          if (typePos != std::string::npos) {
            size_t typeEnd = newParams.find(",", typePos);
            if (typeEnd == std::string::npos) typeEnd = newParams.find(">", typePos);
            if (typeEnd != std::string::npos) {
              std::string integralParam = newParams.substr(typePos, typeEnd - typePos);
              //Replace with trait access (convert to bool-like condition)
              std::string traitAccess = "typename Trait<" + typeParam + ">::value";
              newParams.replace(typePos, integralParam.length(), traitAccess);
              break;
            }
          }
        }
      }
      
      //Replace the template parameter list in the mutated text
      mutatedText.replace(templateStart, templateEnd - templateStart + 1, newParams);
      
      //Also need to add trait template declaration if not present
      //Check if trait template already exists
      bool traitExists = false;
      for (const auto *decl : templateDecls) {
        if (decl->getNameAsString() == "Trait") {
          traitExists = true;
          break;
        }
      }
      
      if (!traitExists) {
        //Insert trait template before the specialization
        std::string traitDecl = "\ntemplate<typename T> struct Trait { static const bool value = true; };";
        SourceLocation insertLoc = CTS->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, traitDecl);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(specializationRange), mutatedText);
    }
}
  
void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher specializationMatcher = classTemplateSpecializationDecl().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}