//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Type_In_Template_Template_Parameter_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templateDecls;
        std::vector<const clang::ClassTemplateDecl *> classTemplateDecls;
        std::vector<const clang::TypeDecl *> convertibleTypes;
    };
};

//source file
#include "../include/Replace_Template_Parameter_Type_In_Template_Template_Parameter_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParam")) {
      //Filter nodes in header files
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateParamType = TT->getTemplateParameters()->getParam(0);
      if (!templateParamType || !isa<clang::NonTypeTemplateParmDecl>(templateParamType))
        return;
      
      auto nonTypeParam = dyn_cast<clang::NonTypeTemplateParmDecl>(templateParamType);
      if (!nonTypeParam)
        return;
      
      auto paramType = nonTypeParam->getType();
      if (!paramType->isClassType())
        return;
      
      const clang::CXXRecordDecl *paramClass = paramType->getAsCXXRecordDecl();
      if (!paramClass)
        return;
      
      //Record convertible types
      for (const auto *typeDecl : convertibleTypes) {
        if (typeDecl == paramClass)
          continue;
        
        const clang::CXXRecordDecl *convertibleClass = dyn_cast<clang::CXXRecordDecl>(typeDecl);
        if (!convertibleClass)
          continue;
        
        //Check for constexpr constructor or conversion function
        bool isConvertible = false;
        for (auto constructor : convertibleClass->ctors()) {
          if (constructor->isConstexpr() && constructor->getNumParams() == 1) {
            auto paramType = constructor->getParamDecl(0)->getType();
            if (paramType->isClassType() && paramType->getAsCXXRecordDecl() == paramClass) {
              isConvertible = true;
              break;
            }
          }
        }
        
        if (!isConvertible) {
          for (auto method : convertibleClass->methods()) {
            if (method->isConstexpr() && method->getNumParams() == 0 && method->getReturnType()->isClassType()) {
              if (method->getReturnType()->getAsCXXRecordDecl() == paramClass) {
                isConvertible = true;
                break;
              }
            }
          }
        }
        
        if (isConvertible) {
          //Find a matching template declaration
          for (const auto *classTemplate : classTemplateDecls) {
            auto templateParams = classTemplate->getTemplateParameters();
            if (templateParams->size() == 1) {
              auto firstParam = templateParams->getParam(0);
              if (isa<clang::NonTypeTemplateParmDecl>(firstParam)) {
                auto nonType = dyn_cast<clang::NonTypeTemplateParmDecl>(firstParam);
                if (nonType && nonType->getType()->isClassType()) {
                  auto templateParamClass = nonType->getType()->getAsCXXRecordDecl();
                  if (templateParamClass == paramClass) {
                    //Perform mutation: create new template with different type
                    std::string newTemplateName = classTemplate->getNameAsString() + "_mut";
                    std::2string newTemplateDecl = "template<" + convertibleClass->getNameAsString() + "> struct " + newTemplateName + " {};\n";
                    
                    //Replace the template argument in the instantiation
                    auto parentDecl = TT->getDeclContext();
                    if (parentDecl && isa<clang::TemplateDecl>(parentDecl)) {
                      auto parentTemplate = dyn_cast<clang::TemplateDecl>(parentDecl);
                      std::string parentText = stringutils::rangetoStr(*(Result.SourceManager), parentTemplate->getSourceRange());
                      std::string mutatedText = parentText;
                      std::string oldTemplateName = classTemplate->getNameAsString();
                      size_t pos = mutatedText.find(oldTemplateName);
                      if (pos != std::string::npos) {
                        mutatedText.insert(pos, newTemplateDecl);
                        mutatedText.replace(pos + newTemplateDecl.length(), oldTemplateName.length(), newTemplateName);
                        mutatedText = "/*mut391*/" + mutatedText;
                        //Replace the original AST node with the mutated one
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(parentTemplate->getSourceRange()), mutatedText);
                      }
                    }
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      templateDecls.push_back(TD);
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplateDecl")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      classTemplateDecls.push_back(CTD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      convertibleTypes.push_back(TD);
    }
}
  
void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateTemplateMatcher = templateTemplateParmDecl().bind("TemplateTemplateParam");
    DeclarationMatcher templateDeclMatcher = decl().bind("TemplateDecl");
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplateDecl");
    DeclarationMatcher typeDeclMatcher = typeDecl().bind("TypeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateTemplateMatcher, &callback);
    matchFinder.addMatcher(templateDeclMatcher, &callback);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(typeDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}