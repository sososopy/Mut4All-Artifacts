//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Member_Access_With_Static_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> visitedClasses;
        std::map<std::string, std::vector<const clang::FieldDecl *>> staticMembers;
        std::map<std::string, std::vector<const clang::FunctionDecl *>> staticMethods;
    };
};

//source file
#include "../include/Mutator_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeType>("DecltypeExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      // Get the underlying expression
      const Expr *E = MT->getUnderlyingExpr();
      if (!E) return;
      
      // Check if it's a member access expression with ::
      if (const DependentScopeDeclRefExpr *DSDRE = dyn_cast<DependentScopeDeclRefExpr>(E)) {
        // Get the nested name specifier
        const NestedNameSpecifier *NNS = DSDRE->getQualifier();
        if (!NNS) return;
        
        // Get the class name from the nested name specifier
        const Type *T = NNS->getAsType();
        if (!T) return;
        
        const CXXRecordDecl *RD = T->getAsCXXRecordDecl();
        if (!RD) return;
        
        // Get the member name
        DeclarationNameInfo DNI = DSDRE->getNameInfo();
        std::string memberName = DNI.getAsString();
        
        // Check if this member is static in the original class
        bool foundStatic = false;
        std::string staticMemberName;
        
        // Look for static fields
        if (staticMembers.find(RD->getNameAsString()) != staticMembers.end()) {
            for (const FieldDecl *FD : staticMembers[RD->getNameAsString()]) {
                if (FD->getNameAsString() != memberName && FD->getType().getAsString() == DSDRE->getType().getAsString()) {
                    foundStatic = true;
                    staticMemberName = FD->getNameAsString();
                    break;
                }
            }
        }
        
        // Look for static methods
        if (!foundStatic && staticMethods.find(RD->getNameAsString()) != staticMethods.end()) {
            for (const FunctionDecl *FD : staticMethods[RD->getNameAsString()]) {
                if (FD->getNameAsString() != memberName) {
                    // Check if return types are compatible
                    std::string originalType = DSDRE->getType().getAsString();
                    std::string staticType = FD->getReturnType().getAsString();
                    
                    // Simple type compatibility check
                    if (originalType == staticType || 
                        (originalType.find("void") != std::string::npos && staticType.find("void") != std::string::npos)) {
                        foundStatic = true;
                        staticMemberName = FD->getNameAsString();
                        break;
                    }
                }
            }
        }
        
        // If no static member found, create one
        if (!foundStatic) {
            // Create a static member name
            staticMemberName = "StaticMut_" + memberName;
            
            // Determine the type for the new static member
            std::string memberType = DSDRE->getType().getAsString();
            
            // Create static member declaration
            std::string staticDecl;
            if (DSDRE->getType()->isFunctionType()) {
                // It's a method, create static method
                staticDecl = "\n\tpublic:\n\tstatic " + memberType + " " + staticMemberName + "() {}\n";
            } else {
                // It's a field, create static field
                staticDecl = "\n\tpublic:\n\tstatic " + memberType + " " + staticMemberName + ";\n";
            }
            
            // Insert the static member declaration at the end of the class
            SourceLocation insertLoc = RD->getEndLoc().getLocWithOffset(-1);
            Rewrite.InsertText(insertLoc, staticDecl);
            
            // If it's a field, also add definition outside the class
            if (!DSDRE->getType()->isFunctionType()) {
                std::string def = "\n" + memberType + " " + RD->getNameAsString() + "::" + staticMemberName + ";\n";
                Rewrite.InsertTextAfterToken(RD->getEndLoc(), def);
            }
        }
        
        // Get the source code text of target node
        std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                          CharSourceRange::getTokenRange(E->getSourceRange()));
        
        // Replace the member name with static member name
        size_t pos = originalText.rfind("::");
        if (pos != std::string::npos) {
            std::string newText = originalText.substr(0, pos + 2) + staticMemberName;
            
            // If it was a function call, preserve the parentheses
            if (originalText.find('(', pos) != std::string::npos) {
                newText += "()";
            }
            
            //Perform mutation on the source code text by applying string replacement
            newText = "/*mut37*/" + newText;
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(E->getSourceRange()), newText);
        }
      }
    }
    // Also collect class declarations to find static members
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
            return;
        
        if (!CD->isCompleteDefinition())
            return;
        
        // Store the class for later reference
        visitedClasses.push_back(CD);
        
        // Collect static members from this class
        std::string className = CD->getNameAsString();
        
        for (const Decl *D : CD->decls()) {
            if (const FieldDecl *FD = dyn_cast<FieldDecl>(D)) {
                if (FD->isStatic()) {
                    staticMembers[className].push_back(FD);
                }
            } else if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
                if (FD->isStatic()) {
                    staticMethods[className].push_back(FD);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = decltypeType().bind("DecltypeExpr");
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}