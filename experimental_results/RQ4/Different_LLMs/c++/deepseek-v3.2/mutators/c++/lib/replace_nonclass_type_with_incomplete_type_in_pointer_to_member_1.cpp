//source file
#include "../include/Replace_NonClass_Type_With_Incomplete_Type_In_Pointer_To_Member_1.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/TypeLoc.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = *Result.SourceManager;
    
    // First pass: collect incomplete non-class types
    if (auto *TD = Result.Nodes.getNodeAs<TagDecl>("incompleteNonClassType")) {
        if (!SM.isWrittenInMainFile(TD->getLocation()))
            return;
        if (!TD->isCompleteDefinition()) {
            std::string typeName = TD->getNameAsString();
            if (!typeName.empty()) {
                if (isa<EnumDecl>(TD)) hasIncompleteEnum = true;
                else if (isa<RecordDecl>(TD) && cast<RecordDecl>(TD)->isUnion()) hasIncompleteUnion = true;
                if (std::find(incompleteNonClassTypes.begin(), incompleteNonClassTypes.end(), typeName) == incompleteNonClassTypes.end()) {
                    incompleteNonClassTypes.push_back(typeName);
                }
            }
        }
        return;
    }
    
    // Second pass: mutate template arguments
    if (auto *TA = Result.Nodes.getNodeAs<TemplateArgumentLoc>("templateArg")) {
        if (!SM.isWrittenInMainFile(TA->getLocation()))
            return;
        
        const TemplateArgument &Arg = TA->getArgument();
        if (Arg.getKind() != TemplateArgument::Type)
            return;
        
        QualType Type = Arg.getAsType();
        if (Type.isNull())
            return;
        
        // Check if this type is used in a pointer-to-member context
        if (Result.Nodes.getNodeAs<DeclRefExpr>("ptrToMemberRef")) {
            // Get the source range of the template argument
            SourceRange range = TA->getSourceRange();
            if (!range.isValid())
                return;
            
            std::string replacementType;
            
            // Try to use existing incomplete non-class type
            if (!incompleteNonClassTypes.empty()) {
                size_t idx = getrandom::getRandomIndex(incompleteNonClassTypes.size() - 1);
                replacementType = incompleteNonClassTypes[idx];
            } else {
                // Create a new forward declaration if none exists
                int choice = getrandom::getRandomIndex(1);
                if (choice == 0 || !hasIncompleteEnum) {
                    replacementType = "Mut1_IncompleteEnum";
                    // Insert forward declaration before the template instantiation
                    SourceLocation insertLoc = SM.getLocForStartOfFile(SM.getMainFileID());
                    std::string forwardDecl = "enum struct Mut1_IncompleteEnum;\n";
                    Rewrite.InsertText(insertLoc, forwardDecl);
                    hasIncompleteEnum = true;
                } else {
                    replacementType = "Mut1_IncompleteUnion";
                    SourceLocation insertLoc = SM.getLocForStartOfFile(SM.getMainFileID());
                    std::string forwardDecl = "union Mut1_IncompleteUnion;\n";
                    Rewrite.InsertText(insertLoc, forwardDecl);
                    hasIncompleteUnion = true;
                }
            }
            
            // Perform the replacement
            std::string originalText = stringutils::rangetoStr(SM, range);
            std::string mutatedText = "/*mut1*/" + replacementType;
            Rewrite.ReplaceText(range, mutatedText);
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    
    // First matcher to collect incomplete non-class types
    DeclarationMatcher incompleteMatcher = tagDecl(unless(anyOf(isDefinition(), isClass())), 
                                                   unless(hasName("")),
                                                   anyOf(isEnum(), isUnion())).bind("incompleteNonClassType");
    
    // Second matcher to find template arguments in pointer-to-member contexts
    StatementMatcher templateArgMatcher = declRefExpr(
        hasType(pointerType(pointee(type().bind("memberType")))),
        hasAncestor(templateArgumentLoc().bind("templateArg"))
    ).bind("ptrToMemberRef");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(incompleteMatcher, &callback);
    matchFinder.addMatcher(templateArgMatcher, &callback);
    matchFinder.matchAST(Context);
}