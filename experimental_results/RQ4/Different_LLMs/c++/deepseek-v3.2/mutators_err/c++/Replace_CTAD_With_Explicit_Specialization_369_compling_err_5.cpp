//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_CTAD_With_Explicit_Specialization_369
 */ 
class MutatorFrontendAction_369 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(369)

private:
    class MutatorASTConsumer_369 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_369(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> availableTemplates;
        std::vector<const clang::TypeDecl *> availableTypes;
    };
};

//source file
#include "../include/Replace_CTAD_With_Explicit_Specialization_369.h"

// ========================================================================================================
#define MUT369_OUTPUT 1

void MutatorFrontendAction_369::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("CTADConstruct")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;

      const clang::CXXConstructorDecl *Ctor = CE->getConstructor();
      if (!Ctor) return;

      const clang::Type *Type = Ctor->getParent()->getTypeForDecl();
      if (!Type) return;

      const clang::TemplateSpecializationType *TST = Type->getAs<clang::TemplateSpecializationType>();
      if (!TST) return;

      // Check if this is a CTAD call (no explicit template arguments)
      if (TST->getNumArgs() > 0) return; // Already explicit

      const clang::TemplateDecl *Template = TST->getTemplateName().getAsTemplateDecl();
      if (!Template) return;

      // Get constructor argument types
      std::vector<clang::QualType> ArgTypes;
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        ArgTypes.push_back(CE->getArg(i)->getType());
      }

      // Deduce template arguments
      std::vector<std::string> DeducedArgs;
      for (const clang::QualType &QT : ArgTypes) {
        const clang::Type *ArgType = QT.getTypePtr();
        std::string TypeStr = ArgType->getCanonicalTypeInternal().getAsString();
        DeducedArgs.push_back(TypeStr);
      }

      // For template template parameters, find an existing template
      std::vector<std::string> FinalArgs;
      const clang::TemplateParameterList *TPL = Template->getTemplateParameters();
      for (unsigned i = 0; i < TPL->size(); ++i) {
        const clang::NamedDecl *Param = TPL->getParam(i);
        if (Param->getKind() == clang::Decl::TemplateTemplateParm) {
          // Try to find an existing template
          std::string TemplateName;
          if (availableTemplates.empty()) {
            // Create placeholder if none exist
            TemplateName = "template<typename> struct MutatedTP {}";
            // Insert placeholder declaration
            Rewrite.InsertTextBefore(CE->getBeginLoc(), TemplateName + ";\n");
          } else {
            size_t idx = getrandom::getRandomIndex(availableTemplates.size() - 1);
            TemplateName = availableTemplates[idx]->getNameAsString();
          }
          FinalArgs.push_back(TemplateName);
        } else {
          // Use deduced type or fallback
          if (i < DeducedArgs.size()) {
            FinalArgs.push_back(DeducedArgs[i]);
          } else {
            // Use a default type
            FinalArgs.push_back("int");
          }
        }
      }

      // Build new explicit specialization string
      std::string NewSpecialization = Template->getNameAsString();
      if (!FinalArgs.empty()) {
        NewSpecialization += "<";
        for (size_t i = 0; i < FinalArgs.size(); ++i) {
          NewSpecialization += FinalArgs[i];
          if (i != FinalArgs.size() - 1) NewSpecialization += ", ";
        }
        NewSpecialization += ">";
      }

      //Get the source code text of target node
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = NewSpecialization + OriginalText.substr(OriginalText.find('('));
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), MutatedText);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      // Collect available templates and types for later use
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      availableTemplates.push_back(TD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      availableTypes.push_back(TD);
    }
}
  
void MutatorFrontendAction_369::MutatorASTConsumer_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher CTADMatcher = cxxConstructExpr(hasType(templateSpecializationType())).bind("CTADConstruct");
    DeclarationMatcher TemplateMatcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    DeclarationMatcher TypeMatcher = clang::ast_matchers::typeDecl().bind("TypeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(CTADMatcher, &callback);
    matchFinder.addMatcher(TemplateMatcher, &callback);
    matchFinder.addMatcher(TypeMatcher, &callback);
    matchFinder.matchAST(Context);
}