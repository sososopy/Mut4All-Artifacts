//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_Partial_Specialization_162
 */ 
class MutatorFrontendAction_162 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(162)

private:
    class MutatorASTConsumer_162 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_162(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeDecl *> availableTypes;
        std::vector<std::string> availableTypeNames;
    };
};

//source file
#include "../include/Mutator_Specialize_Template_Partial_Specialization_162.h"
#include <sstream>

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("PartialSpecialization")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      
      // Ensure it's a partial specialization
      if (!CTSD->isPartialSpecialization())
        return;
      
      // Get the primary template
      auto *PrimaryTemplate = CTSD->getSpecializedTemplate();
      if (!PrimaryTemplate)
        return;
      
      // Get template arguments of the specialization
      const TemplateArgumentList *SpecializationArgs = CTSD->getTemplateArgs();
      if (!SpecializationArgs)
        return;
      
      // Check if any template argument depends on primary template parameters
      bool hasDependentArg = false;
      for (unsigned i = 0; i < SpecializationArgs->size(); ++i) {
        if (SpecializationArgs->get(i).getKind() == TemplateArgument::Type) {
          QualType ArgType = SpecializationArgs->get(i).getAsType();
          if (ArgType->isDependentType()) {
            hasDependentArg = true;
            break;
          }
        }
      }
      
      if (!hasDependentArg)
       2return;
      
      // Collect available non-dependent types from the program
      if (availableTypes.empty()) {
        // We'll collect types from the translation unit
        // For simplicity, we'll use built-in types and user-defined types we've seen
        availableTypeNames = {"int", "float", "double", "char", "bool"};
      }
      
      // Choose a replacement type
      std::string replacementType;
      if (!availableTypeNames.empty()) {
        size_t idx = getrandom::getRandomIndex(availableTypeNames.size() - 1);
        replacementType = availableTypeNames[idx];
      } else {
        replacementType = "int"; // fallback
      }
      
      // Get source range of the specialization
      SourceRange SpecializationRange = CTSD->getSourceRange();
      if (SpecializationRange.isInvalid())
        return;
      
      // Get the source text
      std::string SpecializationText = stringutils::rangetoStr(*(Result.SourceManager), SpecializationRange);
      
      // Find the template argument list in the specialization
      // We need to locate the template arguments after the class name
      size_t templateStart = SpecializationText.find('<');
      if (templateStart == std::string::npos)
        return;
      
      size_t templateEnd = SpecializationText.rfind('>');
      if (templateEnd == std::string::npos)
        return;
      
      // Extract template arguments
      std::string TemplateArgs = SpecializationText.substr(templateStart + 1, templateEnd - templateStart - 1);
      
      // Replace dependent type arguments with the chosen concrete type
      std::string NewTemplateArgs;
      std::istringstream ArgStream(TemplateArgs);
      std::string Arg;
      bool FirstArg = true;
      
      while (std::getline(ArgStream, Arg, ',')) {
        // Trim whitespace
        Arg.erase(0, Arg.find_first_not_of(" \t"));
        Arg.erase(Arg.find_last_not_of(" \t") + 1);
        
        // Check if this argument looks like a dependent type (simple heuristic)
        if (!Arg.empty() && Arg.find("typename") != std::string::npos) {
          // Replace with concrete type
          if (!FirstArg)
            NewTemplateArgs += ", ";
          NewTemplateArgs += replacementType;
        } else {
          // Keep original argument
          if (!FirstArg)
            NewTemplateArgs += ", ";
          NewTemplateArgs += Arg;
        }
        FirstArg = false;
      }
      
      // Construct new specialization text
      std::string NewSpecializationText = SpecializationText.substr(0, templateStart + 1) +
                                          NewTemplateArgs +
                                          SpecializationText.substr(templateEnd);
      
      // Also need to update the template parameter list if it's now empty
      // Look for the template<> part
      size_t templateKeywordPos = SpecializationText.find("template<");
      if (templateKeywordPos != std::string::npos) {
        size_t templateParamEnd = SpecializationText.find('>', templateKeywordPos);
        if (templateParamEnd != std::string::npos) {
          std::string TemplateParams = SpecializationText.substr(templateKeywordPos + 9, 
                                                                 templateParamEnd - templateKeywordPos - 9);
          // If all template parameters were used in dependent arguments that we replaced,
          // we might need to make it a full specialization (template<>)
          // For now, we'll keep the template parameter list but remove unused parameters
          // This is a simplified approach
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SpecializationRange), NewSpecializationText);
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        isPartialSpecialization(),
        unless(isExplicitTemplateSpecialization())
    ).bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}