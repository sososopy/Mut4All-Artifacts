//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Specialization_Of_Inherited_Constructor_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXConstructorDecl*> specializedCtors;
    };
};

//source file
#include "../include/Replace_Explicit_Specialization_Of_Inherited_Constructor_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ES = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ExplicitSpecialization")) {
      //Filter nodes in header files
      if (!ES || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ES->getLocation()))
        return;
      
      // Only consider constructor specializations
      if (!ES->isTemplateInstantiation())
        return;
      
      // Get the specialized constructor
      for (auto *Decl : ES->decls()) {
        if (auto *Ctor = dyn_cast<clang::CXXConstructorDecl>(Decl)) {
          specializedCtors.push_back(Ctor);
        }
      }
    }
    else if (auto *DC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!DC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DC->getLocation()))
        return;
      
      if (!DC->isCompleteDefinition())
        return;
      
      // Check if this derived class has using Base::Base declaration
      bool hasUsingBaseCtor = false;
      for (auto *Decl : DC->decls()) {
        if (auto *UD = dyn_cast<clang::UsingDecl>(Decl)) {
          if (UD->getQualifier() && UD->getQualifier()->getAsType()) {
            auto *QT = UD->getQualifier()->getAsType()->getAsCXXRecordDecl();
            if (QT && DC->isDerivedFrom(QT)) {
              hasUsingBaseCtor = true;
              break;
            }
          }
        }
      }
      
      if (!hasUsingBaseCtor)
        return;
      
      // Find matching specialized constructors for this derived class
      for (auto *Ctor : specializedCtors) {
        auto *Parent = dyn_cast<clang::CXXRecordDecl>(Ctor->getParent());
        if (!Parent)
          continue;
        
        // Check if the specialized constructor belongs to this derived class
        if (Parent->getQualifiedNameAsString() != DC->getQualifiedNameAsString())
          continue;
        
        // Get the base class
        auto *BaseClass = DC->bases_begin()->getType()->getAsCXXRecordDecl();
        if (!BaseClass)
          continue;
        
        // Check if base class has a matching constructor template
        bool baseHasTemplateCtor = false;
        for (auto *BaseDecl : BaseClass->decls()) {
          if (auto *BaseCtor = dyn_cast<clang::CXXConstructorDecl>(BaseDecl)) {
            if (BaseCtor->getNumParams() == Ctor->getNumParams()) {
              // Check if base constructor is templated
              if (BaseCtor->getPrimaryTemplate()) {
                baseHasTemplateCtor = true;
                break;
              }
            }
          }
        }
        
        if (!baseHasTemplateCtor)
          continue;
        
        //Get the source code text of target node
        auto ctorRange = Ctor->getSourceRange();
        if (ctorRange.isInvalid())
          continue;
        
        // Get the parameter types as string
        std::string paramStr;
        for (unsigned i = 0; i < Ctor->getNumParams(); ++i) {
          auto *Param = Ctor->getParamDecl(i);
          if (i > 0) paramStr += ", ";
          paramStr += Param->getType().getAsString();
          if (Param->getName().size() > 0) {
            paramStr += " " + Param->getName().str();
          }
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string specializationDecl = "template<> " + DC->getNameAsString() + 
                                         "(" + paramStr + ");\n";
        
        //Insert the explicit specialization declaration inside the derived class
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(DC->getEndLoc().getLocWithOffset(-1), 
                                 "/*mut3*/" + specializationDecl);
        
        // Remove the original external specialization if it exists
        if (Ctor->isOutOfLine()) {
          Rewrite.RemoveText(ctorRange);
        }
      }
    }
}
  
void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateSpecializationDecl().bind("ExplicitSpecialization");
    DeclarationMatcher matcher2 = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}