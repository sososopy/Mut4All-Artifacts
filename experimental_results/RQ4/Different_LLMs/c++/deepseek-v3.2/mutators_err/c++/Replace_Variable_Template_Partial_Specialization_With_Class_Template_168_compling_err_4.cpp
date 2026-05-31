//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variable_Template_Partial_Specialization_With_Class_Template_168
 */ 
class MutatorFrontendAction_168 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(168)

private:
    class MutatorASTConsumer_168 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_168(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::map<std::string, std::string> varToWrapperMap;
        std::map<std::string, std::vector<clang::SourceRange>> usageRanges;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_Replace_Variable_Template_Partial_Specialization_With_Class_Template_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("varTemplateDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if it's a static constexpr variable template
      if (!VD->getTemplatedDecl()->getType().isConstQualified() || 
          VD->getTemplatedDecl()->getStorageClass() != clang::SC_Static)
        return;
      
      // Get the variable template name
      std::string varName = VD->getNameAsString();
      
      // Generate wrapper name
      std::string wrapperName = varName + "_wrapper";
      int suffixCounter = 1;
      while (!Result.Context->getTranslationUnitDecl()->lookup(clang::DeclarationName(&Result.Context->Idents.get(wrapperName))).empty()) {
        wrapperName = varName + "_wrapper_" + std::to_string(suffixCounter++);
      }
      
      // Store mapping
      varToWrapperMap[varName] = wrapperName;
      
      // Get all partial specializations
      std::string primaryTemplate;
      std::vector<std::string> specializations;
      
      // Get primary template
      if (auto *TemplatedVar = VD->getTemplatedDecl()) {
        auto varRange = TemplatedVar->getSourceRange();
        if (varRange.isValid()) {
          primaryTemplate = stringutils::rangetoStr(*(Result.SourceManager), varRange);
        }
      }
      
      // Get partial specializations
      for (auto *Spec : VD->specializations()) {
        if (Spec->getSpecializationKind() == clang::TemplateSpecializationKind::TSK_ExplicitSpecialization) {
          auto specRange = Spec->getSourceRange();
          if (specRange.isValid()) {
            specializations.push_back(stringutils::rangetoStr(*(Result.SourceManager), specRange));
          }
        }
      }
      
      // Transform primary template to class template
      if (!primaryTemplate.empty()) {
        // Find the variable declaration part
        size_t eqPos = primaryTemplate.find('=');
        if (eqPos != std::string::npos) {
          std::string beforeEq = primaryTemplate.substr(0, eqPos);
          std::string afterEq = primaryTemplate.substr(eqPos);
          
          // Replace variable declaration with struct declaration
          size_t varNamePos = beforeEq.find(varName);
          if (varNamePos != std::string::npos) {
            std::string newDecl = beforeEq.substr(0, varNamePos) + 
                                 "struct " + wrapperName + 
                                 beforeEq.substr(varNamePos + varName.length()) + 
                                 " { static constexpr bool value" + afterEq + "; };";
            
            // Replace the original declaration
            Rewrite.ReplaceText(VD->getTemplatedDecl()->getSourceRange(), newDecl);
          }
        }
      }
      
      // Transform specializations
      for (const auto& spec : specializations) {
        // Find the specialization pattern
        size_t varSpecPos = spec.find(varName + "<");
        if (varSpecPos != std::string::npos) {
          std::string beforeSpec = spec.substr(0, varSpecPos);
          std::string afterSpec = spec.substr(varSpecPos + varName.length());
          
          // Replace variable specialization with struct specialization
          std::string newSpec = beforeSpec + "struct " + wrapperName + afterSpec;
          
          // Replace "= true/false;" with "{ static constexpr bool value = true/false; };"
          size_t eqPos = newSpec.find('=');
          if (eqPos != std::string::npos) {
            std::string valuePart = newSpec.substr(eqPos + 1);
            valuePart.erase(std::remove(valuePart.begin(), valuePart.end(), ';'), valuePart.end());
            valuePart.erase(std::remove(valuePart.begin(), valuePart.end(), ' '), valuePart.end());
            
            newSpec = newSpec.substr(0, eqPos) + 
                     "{ static constexpr bool value = " + valuePart + "; };";
          }
          
          // Find and replace the specialization in the source
          // Note: This is simplified - in practice we'd need to find the exact source range
          // of each specialization
        }
      }
      
    } else if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("varTemplateUsage")) {
      // Handle usage sites
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getLocation()))
        return;
      
      if (auto *VD = llvm::dyn_cast<clang::VarTemplateSpecializationDecl>(DRE->getDecl())) {
        std::string varName = VD->getDeclName().getAsString();
        
        if (varToWrapperMap.find(varName) != varToWrapperMap.end()) {
          // Record usage for later replacement
          usageRanges[varName].push_back(DRE->getSourceRange());
        }
      }
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varTemplateMatcher = clang::ast_matchers::varDecl(clang::ast_matchers::isTemplateInstantiation()).bind("varTemplateDecl");
    StatementMatcher usageMatcher = clang::ast_matchers::declRefExpr(clang::ast_matchers::to(clang::ast_matchers::varDecl(clang::ast_matchers::isTemplateInstantiation()))).bind("varTemplateUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varTemplateMatcher, &callback);
    matchFinder.addMatcher(usageMatcher, &callback);
    matchFinder.matchAST(Context);
    
    // Process recorded usage ranges after AST traversal
    for (const auto& entry : callback.varToWrapperMap) {
      const std::string& varName = entry.first;
      const std::string& wrapperName = entry.second;
      
      for (const auto& range : callback.usageRanges[varName]) {
        if (range.isValid()) {
          std::string usageText = stringutils::rangetoStr(Context.getSourceManager(), range);
          
          // Replace var<...> with wrapper<...>::value
          size_t templateStart = usageText.find('<');
          if (templateStart != std::string::npos) {
            std::string newUsage = wrapperName + usageText.substr(varName.length());
            
            // Check if ::value is already present
            if (newUsage.find("::value") == std::string::npos) {
              size_t templateEnd = newUsage.rfind('>');
              if (templateEnd != std::2string::npos) {
                newUsage.insert(templateEnd + 1, "::value");
              }
            }
            
            TheRewriter.ReplaceText(range, newUsage);
          }
        }
      }
    }
}