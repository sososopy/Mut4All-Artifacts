//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_CTAD_With_Explicit_Template_Args_85
 */ 
class MutatorFrontendAction_85 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(85)

private:
    class MutatorASTConsumer_85 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_85(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> candidateVars;
        std::vector<const clang::ClassTemplateDecl *> constrainedTemplates;
    };
};

//source file
#include "../include/Replace_Constrained_CTAD_With_Explicit_Template_Args_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if this is a CTAD usage (deduced from initializer)
      if (!VD->getType()->getAs<clang::DeducedTemplateSpecializationType>())
        return;
      // Check if the initializer is a CXXConstructExpr
      auto *Init = VD->getInit();
      if (!Init)
        return;
      auto *CE = llvm::dyn_cast<clang::CXXConstructExpr>(Init);
      if (!CE)
        return;
      auto *Ctor = CE->getConstructor();
      if (!Ctor)
        return;
      auto *Record = Ctor->getParent();
      if (!Record)
        return;
      // Check if the class is a template specialization
      auto *Spec = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(Record);
      if (!Spec)
        return;
      // Check if the template has constraints (concepts)
      auto *Template = Spec->getSpecializedTemplate();
      if (!Template)
        return;
      // Check for constrained template parameters
      bool hasConstraints = false;
      for (auto *Param : Template->getTemplateParameters()->asArray()) {
        if (Param->hasAssociatedConstraints()) {
          hasConstraints = true;
          break;
        }
      }
      if (!hasConstraints)
        return;
      // Record candidate variable and template
      candidateVars.push_back(VD);
      constrainedTemplates.push_back(Template);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      // Check if this template has constraints
      bool hasConstraints = false;
      for (auto *Param : TD->getTemplateParameters()->asArray()) {
        if (Param->hasAssociatedConstraints()) {
          hasConstraints = true;
          break;
        }
      }
      if (hasConstraints) {
        // Record constrained template
        constrainedTemplates.push_back(TD);
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("targetVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Find matching candidate variable
      auto it = std::find(candidateVars.begin(), candidateVars.end(), VD);
      if (it == candidateVars.end())
        return;
      size_t idx = std::distance(candidateVars.begin(), it);
      auto *Template = constrainedTemplates[idx];
      // Get the constructor arguments' types
      auto *Init = VD->getInit();
      if (!Init)
        return;
      auto *CE = llvm::dyn_cast<clang::CXXConstructExpr>(Init);
      if (!CE)
        return;
      // Build explicit template arguments from constructor argument types
      std::string TemplateArgs;
      auto NumParams = Template->getTemplateParameters()->size();
      auto NumArgs = CE->getNumArgs();
      for (unsigned i = 0; i < NumParams; ++i) {
        if (i > 0)
          TemplateArgs += ", ";
        if (i < NumArgs) {
          // Use the type of the i-th constructor argument
          auto ArgType = CE->getArg(i)->getType();
          TemplateArgs += ArgType.getAsString();
        } else {
          // If fewer arguments than parameters, use a placeholder (should not happen for valid CTAD)
          TemplateArgs += "int";
        }
      }
      //Get the source code text of target node
      auto VarRange = VD->getSourceRange();
      auto VarText = stringutils::rangetoStr(*(Result.SourceManager), VarRange);
      // Extract variable name and initializer
      std::string VarName = VD->getNameAsString();
      std::string InitText;
      if (auto *InitExpr = VD->getInit()) {
        auto InitRange = InitExpr->getSourceRange();
        InitText = stringutils::rangetoStr(*(Result.SourceManager), InitRange);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string NewDecl = Template->getNameAsString() + "<" + TemplateArgs + "> " + VarName + " = " + InitText + ";";
      NewDecl = "/*mut85*/" + NewDecl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarRange), NewDecl);
    }
}
  
void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First pass: collect constrained templates and CTAD variable declarations
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("templateDecl");
    DeclarationMatcher varMatcher = varDecl(hasInitializer(expr())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
    // Second pass: apply mutation to collected variables
    if (!callback.candidateVars.empty()) {
        MatchFinder secondFinder;
        // Create a matcher for each candidate variable by location
        for (auto *VD : callback.candidateVars) {
            auto Loc = VD->getLocation();
            if (Loc.isValid()) {
                // Use a location-based matcher to find the exact variable
                DeclarationMatcher targetMatcher = varDecl(hasLocation(Loc)).bind("targetVar");
                secondFinder.addMatcher(targetMatcher, &callback);
            }
        }
        secondFinder.matchAST(Context);
    }
}