//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Parameter_Pack_With_Additional_Template_Arguments_412
 */ 
class MutatorFrontendAction_412 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(412)

private:
    class MutatorASTConsumer_412 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_412(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl*> templateDecls;
        std::vector<std::string> packTypes;
    };
};

//source file
#include "../include/Expand_Parameter_Pack_With_Additional_Template_Arguments_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record template declarations with parameter packs
      if (TD->getTemplateParameters()->containsUnexpandedParameterPack()) {
        templateDecls.push_back(TD);
        //Collect types from the parameter pack if identifiable
        for (auto *Param : *TD->getTemplateParameters()) {
          if (Param->isTemplateParameterPack()) {
            if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
              if (TTP->hasDefaultArgument()) {
                //Try to get a default type if available
                auto DefaultArg = TTP->getDefaultArgument();
                if (!DefaultArg.isNull()) {
                  std::string TypeStr = stringutils::rangetoStr(*(Result.SourceManager), DefaultArg.getSourceRange());
                  packTypes.push_back(TypeStr);
                }
              }
            }
          }
        }
      }
    }
    else if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("Specialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocStart()))
        return;
      
      //Check if this specialization matches one of our recorded templates
      const TemplateDecl* matchedDecl = nullptr;
      for (auto* TD : templateDecls) {
        if (TS->getTemplateName().getAsTemplateDecl() == TD) {
          matchedDecl = TD;
          break;
        }
      }
      if (!matchedDecl) return;
      
      //Check if template arguments contain a pack expansion followed by additional arguments
      auto Args = TS->template_arguments();
      bool hasPackExpansion = false;
      int packEndIndex = -1;
      for (unsigned i = 0; i < Args.size(); ++i) {
        if (Args[i].getKind() == TemplateArgument::Pack) {
          hasPackExpansion = true;
          packEndIndex = i;
        }
      }
      if (!hasPackExpansion || packEndIndex == (int)Args.size() - 1) return;
      
      //Get the source code text of target node
      auto sourceRange = TS->getSourceRange();
      if (!sourceRange.isValid()) return;
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Extract template arguments after pack expansion
      std::vector<std::string> additionalArgs;
      for (unsigned i = packEndIndex + 1; i < Args.size(); ++i) {
        auto argRange = Args[i].getSourceRange();
        if (argRange.isValid()) {
          additionalArgs.push_back(stringutils::rangetoStr(*(Result.SourceManager), argRange));
        }
      }
      if (additionalArgs.empty()) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      //Find the position of the pack expansion and following arguments
      //We'll manipulate the additional arguments
      int mutationType = getrandom::getRandomIndex(2); //0: swap, 1: duplicate, 2: insert pack type
      
      if (mutationType == 0 && additionalArgs.size() >= 2) {
        //Swap two arguments after pack expansion
        std::swap(additionalArgs[0], additionalArgs[1]);
      } else if (mutationType == 1) {
        //Duplicate first additional argument
        additionalArgs.insert(additionalArgs.begin() + 1, additionalArgs[0]);
      } else if (mutationType == 2 && !packTypes.empty()) {
        //Insert a type from the pack before the first additional argument
        std::string packType = packTypes[getrandom::getRandomIndex(packTypes.size() - 1)];
        additionalArgs.insert(additionalArgs.begin(), packType);
      }
      
      //Rebuild the template argument list
      //We need to locate the pack expansion and replace the subsequent arguments
      //This is a simplified approach - in practice would need more precise source location handling
      std::string newArgs = "";
      for (unsigned i = 0; i <= (unsigned)packEndIndex; ++i) {
        auto argRange = Args[i].getSourceRange();
        if (argRange.isValid()) {
          newArgs += stringutils::rangetoStr(*(Result.SourceManager), argRange);
          if (i < (unsigned)packEndIndex) newArgs += ", ";
        }
      }
      if (!newArgs.empty() && packEndIndex < (int)Args.size() - 1) newArgs += ", ";
      for (size_t i = 0; i < additionalArgs.size(); ++i) {
        newArgs += additionalArgs[i];
        if (i < additionalArgs.size() - 1) newArgs += ", ";
      }
      
      //Replace the arguments part in the original text
      //Find the template argument list within the specialization
      size_t startPos = mutatedText.find('<');
      size_t endPos = mutatedText.rfind('>');
      if (startPos != std::string::npos && endPos != std::string::npos) {
        mutatedText.replace(startPos + 1, endPos - startPos - 1,newArgs);
      }
      
      mutatedText = "/*mut412*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = clang::ast_matchers::functionTemplateDecl().bind("TemplateDecl");
    TypeMatcher specializationMatcher = clang::ast_matchers::templateSpecializationType().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}